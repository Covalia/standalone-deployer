#ifndef IO_MACOSSHORTCUTIMPL_H
#define IO_MACOSSHORTCUTIMPL_H

#include "factories/shortcut/shortcutimpl.h"

class MacosShortcutImpl : public ShortcutImpl {
    public:
        MacosShortcutImpl();
        virtual ~MacosShortcutImpl();
        virtual bool createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName);
        virtual bool createStartShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName);
        virtual bool createStartMenuShortcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName);
};

#endif
