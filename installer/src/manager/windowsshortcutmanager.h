#ifndef WINDOWSSHORTCUTMANAGER_H
#define WINDOWSSHORTCUTMANAGER_H

#include <QString>

class WindowsShortcutManager
{
    public:
        WindowsShortcutManager();

        bool createDesktopShortcut(QString shortcutName, QString args);
};

#endif // WINDOWSSHORTCUTMANAGER_H
