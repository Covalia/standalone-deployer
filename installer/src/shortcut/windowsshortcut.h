#ifndef INSTALLER__SHORTCUT_H
#define INSTALLER__SHORTCUT_H

#include <QtGlobal>

#ifdef Q_OS_WIN


#include <QString>
#include <Windows.h>

/**
 * \class Shortcut
 *
 * @brief Shortcut création class
 */
    class WindowsShortcut
    {
        public:
            WindowsShortcut();

            /**
             * @brief create windows desktop shortcut
             * @param _shortcutName : name of  shortcut link
             * @param _args : args added to target
             * @return true if success
             */
            bool createDesktopShortcut(QString _shortcutName, QString _args);

            /**
             * @brief create windows shortcut in startup folder to run application when computer starting
             * @param _shortcutName : name of  shortcut link
             * @param _allUser : if startup alluser folder or current user
             * @return true if success
             */
            bool createStartShorcut(QString _shortcutName, bool _allUser);

            /**
             * @brief create startmenu folder and shortcut
             * @param _startMenuFolderName : name of folder
             * @param _allUser : in allsuer startmenu or current user
             * @return true if success
             */
            bool createStartMenuShorcut(QString _startMenuFolderName, bool _allUser);

            /**
             * @brief createShortcut
             * @param shortcutPath - File name of the actual link file being created.
             * @param targetPath - File name of the link's target.
             * @param _args - Command line arguments passed to link's target.
             * @param executionDir - Working directory of the active link.
             * @param iconPath - File name of the icon file used for the link.
             * @param description - Description of the linked item.
             * @return true if success
             */
            bool createShortcut(QString shortcutPath, QString targetPath, QString _args, QString executionDir, QString iconPath, QString description);

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

        private:
            QString findWindowsPath(int hToken);

            /**
             * @brief Creates the actual 'lnk' file (assumes COM has been initialized).
             *
             * @param pszTargetfile     - File name of the link's target.
             * @param pszTargetargs     - Command line arguments passed to link's target.
             * @param pszLinkfile       - File name of the actual link file being created.
             * @param pszDescription    - Description of the linked item.
             * @param iShowmode         - ShowWindow() constant for the link's target.
             * @param pszCurdir         - Working directory of the active link.
             * @param pszIconfile       - File name of the icon file used for the link.
             * @param iIconindex        - Index of the icon in the icon file.
             *
             * @return HRESULT value >= 0 for success, < 0 for failure.
             */
            HRESULT createWindowsShortcut(LPCWSTR _pszTargetfile, LPCWSTR _pszTargetargs,
                                          LPSTR _pszLinkfile, LPCWSTR _pszDescription,
                                          int _iShowmode, LPCWSTR _pszCurdir,
                                          LPCWSTR _pszIconfile, int _iIconindex);
    };

#endif
#endif
