#ifndef IO__MACOSSHORTCUTIMPL_H
#define IO__MACOSSHORTCUTIMPL_H

#include "tools/shortcut/shortcutimpl.h"

class MacosShortcutImpl : public ShortcutImpl {
    public:
        MacosShortcutImpl();
        virtual ~MacosShortcutImpl();
        virtual bool createDesktopShortcut(InstallPath _installPath, QString _shortcutName, QString _args, QString _installLocation, QString _applicationName);
        virtual bool createStartShorcut(InstallPath _installPath, QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName);
        virtual bool createStartMenuShorcut(InstallPath _installPath, QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName);
};

#endif
