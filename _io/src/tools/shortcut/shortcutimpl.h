#ifndef IO__SHORTCUTIMPL_H
#define IO__SHORTCUTIMPL_H

#include "tools/shortcut/shortcut.h"
#include "io/config.h"

#include <QString>

class ShortcutImpl {
    public:
        // Tout le code en commun vient ici
        ShortcutImpl();
        virtual ~ShortcutImpl();
        virtual bool createDesktopShortcut(QString _shortcutName, QString _args, QString _installLocation, QString _applicationName) = 0;
        virtual bool createStartShorcut(QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName) = 0;
        virtual bool createStartMenuShorcut(QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName) = 0;
};

#endif
