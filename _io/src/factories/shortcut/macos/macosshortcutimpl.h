#ifndef IO__MACOSSHORTCUTIMPL_H
#define IO__MACOSSHORTCUTIMPL_H

#include "factories/shortcut/shortcutimpl.h"

class MacosShortcutImpl : public ShortcutImpl {
    public:
        MacosShortcutImpl();
        virtual ~MacosShortcutImpl();
        virtual bool createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _installLocation, QString _applicationName);
        virtual bool createStartShorcut(AppPath _appPath, QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName);
        virtual bool createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName);
};

#endif
