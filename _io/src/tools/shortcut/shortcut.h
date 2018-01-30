#ifndef IO__SHORTCUT_H
#define IO__SHORTCUT_H

#include <QString>
#include "tools/shortcut/shortcutimpl.h"
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

        bool createDesktopShortcut(QString _shortcutName, QString _args, QString _installLocation, QString _applicationName);
        bool createStartShorcut(QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName);
        bool createStartMenuShorcut(QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName);

    private:
        ShortcutImpl * m_shortcutImpl;
};

#endif
