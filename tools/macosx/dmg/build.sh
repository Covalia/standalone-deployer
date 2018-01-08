#!/bin/bash

# inspired by: https://stackoverflow.com/a/1513578/8570451

if [ $# -eq 2 ]; then
  PROJECT=$1
  BACKGROUND_IMAGE=$2
else
  printf 'usage: %s <project> <background_image>\n' "$(basename "$0")" >&2
  exit 1
fi

# path to hdiutil command
HDIUTIL=/usr/bin/hdiutil

# path to this script
SCRIPT_PATH="$( cd "$(dirname "$0")" || exit 2; pwd -P )"

# temporary dmg size
DMG_SIZE=20000

printf 'Building dmg image for %s app\n' "${PROJECT}" >&2

TEMP_DMG=$(mktemp -q -t "${PROJECT}"-XXXX.dmg)

# directory of application to embed into the dmg
APPDIR="$(dirname "$(dirname "$(dirname "$SCRIPT_PATH")")")"/"${PROJECT}"/bin
printf 'Grabbing app from dir: %s\n' "${APPDIR}" >&2

printf 'Removing old dmg image: %s\n' "${APPDIR}/${PROJECT}.dmg" >&2
rm -fv "${APPDIR}/${PROJECT}.dmg"

printf 'Creating temporary dmg image: %s\n' "${TEMP_DMG}" >&2
${HDIUTIL} create -quiet -ov -srcfolder "${APPDIR}" -volname "${PROJECT}" -fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size ${DMG_SIZE}k "${TEMP_DMG}"

DEVICE=$(${HDIUTIL} attach -readwrite -noverify -noautoopen "${TEMP_DMG}" | grep -E '^/dev/' | sed 1q | awk '{print $1}')
printf 'Attached device: %s\n' "${DEVICE}" >&2

sleep 1

printf 'Creating /Volumes/%s/.background directory\n' "${PROJECT}" >&2
mkdir -v /Volumes/"${PROJECT}"/.background/

printf 'Copying background image: %s to /Volumes/%s/.background/\n' "${SCRIPT_PATH}/${BACKGROUND_IMAGE}" "${PROJECT}" >&2
cp -v "${SCRIPT_PATH}/${BACKGROUND_IMAGE}" /Volumes/"${PROJECT}"/.background/

printf 'Configuring dmg image view\n' >&2
echo '
   tell application "Finder"
     tell disk "'"${PROJECT}"'"
           open
           set current view of container window to icon view
           set toolbar visible of container window to false
           set statusbar visible of container window to false
           set the bounds of container window to {400, 400, 850, 850}
           set theViewOptions to the icon view options of container window
           set arrangement of theViewOptions to not arranged
           set icon size of theViewOptions to 128
           set text size of theViewOptions to 16
           set background picture of theViewOptions to file ".background:'${BACKGROUND_IMAGE}'"
           set position of item "'"${PROJECT}.app"'" of container window to {225, 225}
           close
           open
           update without registering applications
           delay 1
           close
     end tell
   end tell
' | osascript

printf 'Setting permissions on dmg content\n' >&2
chmod -Rf go-w /Volumes/"${PROJECT}"

# sync

printf 'Detaching device: %s\n' "${DEVICE}" >&2
${HDIUTIL} detach -quiet "${DEVICE}"

printf 'Converting temporary dmg image to readonly\n' >&2
${HDIUTIL} convert -quiet "${TEMP_DMG}" -format UDZO -imagekey zlib-level=9 -o "${APPDIR}/${PROJECT}.dmg"

printf 'Removing temporary dmg image\n' >&2
rm -fv "${TEMP_DMG}"
