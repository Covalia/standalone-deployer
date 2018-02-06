#ifndef IO__SHORTCUT_H
#define IO__SHORTCUT_H

#include <QString>
#include <QSharedPointer>
#include "tools/shortcut/shortcutimpl.h"
#include "tools/installpath/installpath.h"
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

        bool createDesktopShortcut(InstallPath _installPath, QString _shortcutName, QString _args, QString _installLocation, QString _applicationName);
        bool createStartShorcut(InstallPath _installPath, QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName);
        bool createStartMenuShorcut(InstallPath _installPath, QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName);

    private:
        QSharedPointer<ShortcutImpl> m_shortcutImpl;
};

#endif
