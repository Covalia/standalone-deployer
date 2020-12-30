#ifndef INSTALLER_SHORTCUT_H
#define INSTALLER_SHORTCUT_H

#include <QString>
#include <QSharedPointer>
#include "installerfactories/shortcut/shortcutimpl.h"
#include "factories/apppath/apppath.h"
#include "io/config.h"

class ShortcutImpl;

/**
 * \class Shortcut
 *
 * @brief Shortcut création class
 */
class Shortcut {
    public:
        Shortcut();
        virtual ~Shortcut();

        bool createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName);
        bool createStartShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName);
        bool createStartMenuShortcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName);

    private:
        QSharedPointer<ShortcutImpl> m_shortcutImpl;
};

#endif
