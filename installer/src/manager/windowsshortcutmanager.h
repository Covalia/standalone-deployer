#ifndef WINDOWSSHORTCUTMANAGER_H
#define WINDOWSSHORTCUTMANAGER_H

#ifdef _WIN32

#include <QString>

class WindowsShortcutManager
{
    public:
        WindowsShortcutManager();

        bool createDesktopShortcut(QString _shortcutName, QString _args);
        bool createStartShorcut(QString _shortcutName, bool _allUser);
        bool createStartMenuShorcut(QString _startMenuFolderName, bool _allUser);

    private:
        bool createShortcut(QString shortcutPath, QString targetPath, QString _args, QString executionDir, QString iconPath, QString description);
};
#endif

#endif // WINDOWSSHORTCUTMANAGER_H
