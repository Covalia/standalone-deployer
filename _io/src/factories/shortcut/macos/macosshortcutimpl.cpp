#include "factories/shortcut/macos/macosshortcutimpl.h"

MacosShortcutImpl::MacosShortcutImpl()
{
}

MacosShortcutImpl::~MacosShortcutImpl()
{
}

bool MacosShortcutImpl::createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName)
{
    Q_UNUSED(_appPath)
    Q_UNUSED(_shortcutName)
    Q_UNUSED(_args)
    Q_UNUSED(_applicationName)
    // TODO implement
    return true;
}

bool MacosShortcutImpl::createStartShorcut(AppPath _appPath, QString _shortcutName, QString _applicationName)
{
    Q_UNUSED(_appPath)
    Q_UNUSED(_shortcutName)
    Q_UNUSED(_applicationName)
    // TODO implement
    return true;
}

bool MacosShortcutImpl::createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName)
{
    Q_UNUSED(_appPath)
    Q_UNUSED(_startMenuFolderName)
    Q_UNUSED(_applicationName)
    // TODO implement
    return true;
}
