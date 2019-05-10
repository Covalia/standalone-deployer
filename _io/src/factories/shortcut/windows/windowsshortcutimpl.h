#ifndef IO_WINDOWSSHORTCUTIMPL_H
#define IO_WINDOWSSHORTCUTIMPL_H

#include "factories/shortcut/shortcutimpl.h"

#include <Windows.h>

class WindowsShortcutImpl : public ShortcutImpl {
    public:
        WindowsShortcutImpl();
        virtual ~WindowsShortcutImpl();

        /**
         * @brief create windows desktop shortcut
         * @param _appPath : objet AppPath
         * @param _shortcutName : name of  shortcut link
         * @param _args : args added to target
         * @param _applicationName : name of application
         * @return true if success
         */
        virtual bool createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName);

        /**
         * @brief create windows shortcut in startup folder to run application when computer starting
         * @param _appPath : objet AppPath
         * @param _shortcutName : name of  shortcut link
         * @param _allUser : if startup alluser folder or current user
         * @param _applicationName : name of application
         * @return true if success
         */
        virtual bool createStartShorcut(AppPath _appPath, QString _shortcutName, bool _allUser, QString _applicationName);

        /**
         * @brief create startmenu folder and shortcut
         * @param _appPath : objet AppPath
         * @param _startMenuFolderName : name of folder
         * @param _allUser : in allsuer startmenu or current user
         * @param _applicationName : name of application
         * @return true if success
         */
        virtual bool createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, bool _allUser, QString _applicationName);

    private:
        /**
         * @brief createShortcut
         * @param _shortcutFile - File name of the actual link file being created.
         * @param _targetFile - File name of the link's target.
         * @param _args - Command line arguments passed to link's target.
         * @param _workingDir - Working directory of the active link.
         * @param _iconFile - File name of the icon file used for the link.
         * @param _description - Description of the linked item.
         * @return true if success
         */
        bool createShortcut(QFile &_shortcutFile, const QFile &_targetFile, QString _args, QDir _workingDir, const QFile &_iconPath, QString _description);

        /**
         * @brief Find programs path for all users in windows
         */
        QString findAllUserStartMenuProgramsFolder();

        /**
         * @brief Find startup path for all users in windows
         */
        QString findAllUserStartupFolder();

        /**
         * @brief Find programs path for current user in windows
         */
        QString findUserStartMenuProgramFolder();

        /**
         * @brief Find startup path for current user in windows
         */
        QString  findUserStartupFolder();

        /**
         * @brief Find Windows Paths.
         */
        QString findWindowsPath(int hToken);

        /**
         * brief convert QString to LPCWSTR. Taller is responsible to delete[] the returned pointer.
         */
        LPCWSTR lpcwstrFromQString(QString _qstring);

        /**
         * @brief Creates the actual 'lnk' file (assumes COM has been initialized).
         *
         * @param _targetFile     - File name of the link's target.
         * @param _targetArgs     - Command line arguments passed to link's target.
         * @param _linkFile       - File name of the actual link file being created.
         * @param _description    - Description of the linked item.
         * @param _currentDir     - Working directory of the active link.
         * @param _iconFile       - File name of the icon file used for the link.
         * @param _iconIndex      - Index of the icon in the icon file.
         *
         * @return HRESULT value >= 0 for success, < 0 for failure.
         */
        HRESULT createWindowsShortcut(QString _targetFile, QString _targetArgs,
                                      QString _linkFile, QString _description,
                                      QString _currentDir, QString _iconFile, int _iconIndex);
};

#endif
