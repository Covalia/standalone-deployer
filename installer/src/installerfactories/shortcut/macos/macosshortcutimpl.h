#ifndef INSTALLER_MACOSSHORTCUTIMPL_H
#define INSTALLER_MACOSSHORTCUTIMPL_H

#include "installerfactories/shortcut/shortcutimpl.h"

class MacosShortcutImpl : public ShortcutImpl {
    public:
        MacosShortcutImpl();
        virtual ~MacosShortcutImpl();
        virtual bool createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName);
        virtual bool createStartShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName);
        virtual bool createStartMenuShortcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName);

    private:
        // _shortcutName is the name of the shortcut, _applicationName is the name of
        // the application taken from resources/fixed/macos/application/*
        bool createApplicationShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName);
};

#endif
