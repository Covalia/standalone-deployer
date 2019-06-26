import biplist
import os.path
import sys

# Optional: out_of_dir_app is used to make a dmg outside of the repository. used to build others versions using build scripts.
# dmgbuild -s dmgbuild-settings.py -D background="path/to/background.png" [-D out_of_dir_app="/path/to/my/app.app"] "volume_name" "filename.dmg"

background = defines.get('background')
out_of_dir_app = defines.get('out_of_dir_app')

if not volume_name:
    sys.exit(1)

if not filename:
    sys.exit(2)

if not background:
    sys.exit(3)

appname = '{}.app'.format(volume_name)

if not out_of_dir_app:
    files = ['bin/{}'.format(appname)]
else:
    files = [out_of_dir_app]

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
