#ifndef IO_SHORTCUT_H
#define IO_SHORTCUT_H

#include <QString>
#include <QSharedPointer>
#include "factories/shortcut/shortcutimpl.h"
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
        bool createStartShorcut(AppPath _appPath, QString _shortcutName, QString _applicationName);
        bool createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName);

    private:
        QSharedPointer<ShortcutImpl> m_shortcutImpl;
};

#endif
