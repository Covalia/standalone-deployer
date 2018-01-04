#ifndef INSTALLER__SHORTCUT_H
#define INSTALLER__SHORTCUT_H

#ifdef _WIN32

#include <QString>
#include <Windows.h>


const QString PATH = "C:/Users/Alexis/Desktop/";
const QString FILE_NAME = "test.exe";
const QString PATH_LINK = "C:/Users/Alexis/Desktop/";



/**
 * \class Shortcut
 *
 * @brief Shortcut création class
 */
class Shortcut
{
public:
        Shortcut();

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
        HRESULT createWindowsShortcut(LPCWSTR pszTargetfile, LPCWSTR pszTargetargs,
                                      LPSTR pszLinkfile, LPCWSTR pszDescription,
                                      int iShowmode, LPCWSTR pszCurdir,
                                      LPCWSTR pszIconfile, int iIconindex);

        /**
         * @brief Find start menu path for all users in windows (for add shortcut in start menu)
         * @return path start menu for all users
         */
        QString findAllUserStartMenuFolder();

        /**
         * @brief Find start menu path for current user in windows (for add shortcut in start menu)
         * @return path start menu for current user
         */
        QString findUserStartMenuFolder();
    };

#endif // _WIN32
#endif // INSTALLER__SHORTCUT_H
