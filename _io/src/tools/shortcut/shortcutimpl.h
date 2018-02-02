#ifndef IO__SHORTCUTIMPL_H
#define IO__SHORTCUTIMPL_H

#include "tools/shortcut/shortcut.h"
#include "tools/installpath/installpath.h"
#include "io/config.h"

#include <QString>

class ShortcutImpl {
    public:
        // Tout le code en commun vient ici
        ShortcutImpl();
        virtual ~ShortcutImpl();
        virtual bool createDesktopShortcut(InstallPath _installPath, QString _shortcutName, QString _args, QString _installLocation, QString _applicationName) = 0;
        virtual bool createStartShorcut(InstallPath _installPath, QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName) = 0;
        virtual bool createStartMenuShorcut(InstallPath _installPath, QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName) = 0;
};

#endif
