#ifndef INSTALLER__SHORTCUT_H
#define INSTALLER__SHORTCUT_H

#ifdef _WIN32

#include <QString>
#include <Windows.h>

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
            HRESULT createWindowsShortcut(LPCWSTR _pszTargetfile, LPCWSTR _pszTargetargs,
                                          LPSTR _pszLinkfile, LPCWSTR _pszDescription,
                                          int _iShowmode, LPCWSTR _pszCurdir,
                                          LPCWSTR _pszIconfile, int _iIconindex);

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
    };

#endif
#endif
