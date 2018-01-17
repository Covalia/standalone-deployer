#ifndef WINDOWSSHORTCUTMANAGER_H
#define WINDOWSSHORTCUTMANAGER_H

#include <QString>

class WindowsShortcutManager
{
    public:
        WindowsShortcutManager();

        bool createDesktopShortcut(QString _shortcutName, QString _args);
        bool createStartShorcut(QString _shortcutName, bool allUser);

    private:
        bool createShortcut(QString _shortcutFolderPath, QString _shortcutName, QString _args);
};

#endif // WINDOWSSHORTCUTMANAGER_H
