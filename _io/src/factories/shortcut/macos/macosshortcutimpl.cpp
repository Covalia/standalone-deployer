#include "factories/shortcut/macos/macosshortcutimpl.h"

MacosShortcutImpl::MacosShortcutImpl()
{
}

MacosShortcutImpl::~MacosShortcutImpl()
{
}

bool MacosShortcutImpl::createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _installLocation, QString _applicationName)
{
    // TODO
    return false;
}

bool MacosShortcutImpl::createStartShorcut(AppPath _appPath, QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName)
{
    // TODO
    return false;
}

bool MacosShortcutImpl::createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName)
{
    // TODO
    return false;
}
