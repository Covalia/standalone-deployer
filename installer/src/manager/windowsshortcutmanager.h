#ifndef WINDOWSSHORTCUTMANAGER_H
#define WINDOWSSHORTCUTMANAGER_H

#include <QString>

class WindowsShortcutManager
{
    public:
        WindowsShortcutManager();

        bool createDesktopShortcut(QString _shortcutName, QString _args);
};

#endif // WINDOWSSHORTCUTMANAGER_H
