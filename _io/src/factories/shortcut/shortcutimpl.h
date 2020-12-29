#ifndef IO_SHORTCUTIMPL_H
#define IO_SHORTCUTIMPL_H

#include "factories/shortcut/shortcut.h"
#include "factories/apppath/apppath.h"
#include "io/config.h"

#include <QString>

class ShortcutImpl {
    public:
        // Tout le code en commun vient ici
        ShortcutImpl();
        virtual ~ShortcutImpl();
        virtual bool createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName) = 0;
        virtual bool createStartShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName) = 0;
        virtual bool createStartMenuShortcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName) = 0;
};

#endif
