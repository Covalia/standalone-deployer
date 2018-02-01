#ifndef IO__MACOSSHORTCUTIMPL_H
#define IO__MACOSSHORTCUTIMPL_H

#include "tools/shortcut/shortcutimpl.h"

class MacosShortcutImpl : public ShortcutImpl {
    public:
        MacosShortcutImpl();
        virtual ~MacosShortcutImpl();
        virtual bool createDesktopShortcut(QString _shortcutName, QString _args, QString _installLocation, QString _applicationName);
        virtual bool createStartShorcut(QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName);
        virtual bool createStartMenuShorcut(QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName);
};

#endif
