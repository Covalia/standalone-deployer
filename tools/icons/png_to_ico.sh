#!/bin/bash

# image magick must be installed before.

# path to this script
SCRIPT_PATH="$( cd "$(dirname "$0")" || exit 2; pwd -P )"

if [ $# -ne 1 ]; then
	printf 'usage: %s <project_name>\n' "$(basename "$0")" >&2
	printf 'project name must be the icon name without png extension' >&2
	exit 1
fi

if [ -z "$1" ]; then
	printf 'icon name is empty\n' >&2
	exit 2
elif [ ! -e "${SCRIPT_PATH}/$1.png" ]; then
	printf "png '%s' does not exist\n" "$1" >&2
	exit 3
fi

PROJECT=$1

# magick must be installed
command -v magick > /dev/null 2>&1 || { printf "Command magick is required but it's not installed. Aborting." >&2; exit 4; }

convert "${SCRIPT_PATH}/${PROJECT}".png \
	\( -clone 0 -resize 16x16 \) \
	\( -clone 0 -resize 32x32 \) \
	\( -clone 0 -resize 48x48 \) \
	\( -clone 0 -resize 64x64 \) \
	\( -clone 0 -resize 128x128 \) \
	\( -clone 0 -resize 256x256 \) \
	-delete 0 -alpha on "${SCRIPT_PATH}/${PROJECT}".ico

