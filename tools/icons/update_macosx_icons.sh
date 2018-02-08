#!/bin/bash

# path to this script
SCRIPT_PATH="$( cd "$(dirname "$0")" || exit 2; pwd -P )"

function pack_icon {
    if [ $# -ne 1 ]; then
        printf 'usage: %s <project_name>\n' "$(basename "$0")" >&2
        printf 'project name must be the icon name without png extension' >&2
        exit 1
    fi

    if [ -z "$1" ]; then
        printf 'icon name is empty\n' >&2
        return 2
    elif [ ! -e "${SCRIPT_PATH}/$1.png" ]; then
        printf 'png icon does not exist\n' >&2
        return 3
    fi

    PROJECT=$1

    mkdir "${SCRIPT_PATH}/${PROJECT}".iconset

    sips -z 16 16     "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_16x16.png
    sips -z 32 32     "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_16x16@2x.png
    sips -z 32 32     "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_32x32.png
    sips -z 64 64     "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_32x32@2x.png
    sips -z 128 128   "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_128x128.png
    sips -z 256 256   "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_128x128@2x.png
    sips -z 256 256   "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_256x256.png
    sips -z 512 512   "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_256x256@2x.png
    sips -z 512 512   "${SCRIPT_PATH}/${PROJECT}".png --out "${SCRIPT_PATH}/${PROJECT}".iconset/icon_512x512.png

    iconutil -c icns "${SCRIPT_PATH}/${PROJECT}".iconset
    mv -v "${SCRIPT_PATH}/${PROJECT}".icns "${SCRIPT_PATH}/../../${PROJECT}"/

    rm -rf "${SCRIPT_PATH}/${PROJECT}".iconset
}

# TODO put icon license into the about page
# license: Freepik on flaticon.com
# updater
pack_icon 'updater'
# https://www.flaticon.com/free-icon/cloud-computing_667470
# installer
pack_icon 'installer'
# https://www.flaticon.com/free-icon/box_679821

# license: Those Icons on flaticon.com
# loader
# https://www.flaticon.com/free-icon/busy_522813
pack_icon 'loader'
