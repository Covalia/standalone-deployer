import biplist
import os.path
import sys

# dmgbuild -s dmgbuild-settings.py -D background="path/to/background.png" "volume_name" "filename.dmg"

background = defines.get('background')

if not volume_name:
    sys.exit(1)

if not filename:
    sys.exit(2)

if not background:
    sys.exit(3)

appname = '{}.app'.format(volume_name)
files = ['bin/{}'.format(appname)]

print('Volume name: {}'.format(volume_name))
print('Out filename: {}'.format(filename))
print('Background image: {}'.format(background))
print('App name: {}'.format(appname))
print('Files to package: {}'.format(files))

format = 'UDZO'
size = None
symlinks = {}
icon_locations = {
        appname:        (225, 220)
        }
show_status_bar = False
show_tab_view = False
show_toolbar = False
show_pathbar = False
show_sidebar = False
sidebar_width = 180
window_rect = ((100, 100), (450, 460))
default_view = 'icon-view'
arrange_by = None
scroll_position = (0, 0)
label_pos = 'bottom'
text_size = 16
icon_size = 128
