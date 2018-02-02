#include "tools/shortcut/windows/windowsshortcutimpl.h"

#include <Windows.h>
#include <shlobj.h>
#include <winnls.h>
#include <shobjidl.h>
#include <objbase.h>
#include <objidl.h>
#include <shlguid.h>
#include <Shlobj.h>

#include "log/logger.h"

#include <QStandardPaths>

WindowsShortcutImpl::WindowsShortcutImpl()
{
}

WindowsShortcutImpl::~WindowsShortcutImpl()
{
}

bool WindowsShortcutImpl::createDesktopShortcut(InstallPath _installPath, QString _shortcutName, QString _args, QString _installLocation, QString _applicationName)
{
    L_INFO("Start preparation of desktop shortcut");
    QString loaderPath =  _installPath.getLoaderFilePath();
    QString shortcutPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + _shortcutName + ".lnk";
    QString description = _applicationName;
    QString executionDir = _installLocation;
    QString iconPath = _installPath.getImagesDirPath().absolutePath() + "/shortcut.ico";
    return createShortcut(shortcutPath, loaderPath, _args, executionDir, iconPath, description);
}

bool WindowsShortcutImpl::createStartShorcut(InstallPath _installPath, QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName)
{
    L_INFO("Start creation shortcut in startup folder to run at computer start");
    QString loaderPath =  _installPath.getLoaderFilePath();
    QString description = _applicationName;
    QString executionDir = _installLocation;
    QString iconPath = _installPath.getImagesDirPath().absolutePath() + "/shortcut.ico";
    if (_allUser) {
        return createShortcut(findAllUserStartupFolder() + "/" + _shortcutName + ".lnk", loaderPath, "", executionDir, iconPath, description);
    } else {
        return createShortcut(findUserStartupFolder() + "/" + _shortcutName + ".lnk", loaderPath, "", executionDir, iconPath, description);
    }
}

bool WindowsShortcutImpl::createStartMenuShorcut(InstallPath _installPath, QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName)
{
    L_INFO("Start creation shortcut in startMenu folder");
    QString folderPath("");

    if (_allUser) {
        folderPath = findAllUserStartMenuProgramsFolder();
    } else {
        folderPath = findUserStartMenuProgramFolder();
    }

    // startMenu folder creation
    QString applicationFolder = folderPath + "/" + _startMenuFolderName;

    bool successCreationStartMenuFolder = _installPath.makeDirectoryIfNotExists(folderPath, _startMenuFolderName);

    if (!successCreationStartMenuFolder) {
        L_ERROR("Startmenu folder doesn't exist : " + applicationFolder +  " . Startmenu installation failure");
        return false;
    } else {
        L_INFO("Success Startmenu folder creation " + applicationFolder);
    }

    QFileInfo applicationFolderInfo(applicationFolder);
    if (!applicationFolderInfo.isDir()) {
        L_ERROR("Startmenu folder is not Dir : " + applicationFolder +  " . Startmenu installation failure");
        return false;
    }
    if (!applicationFolderInfo.isWritable()) {
        L_ERROR("Startmenu folder is not writable : " + applicationFolder +  " . Startmenu installation failure");
        return false;
    }

    QString runApplicationPathShortcut =  _applicationName + ".lnk";
    QString uninstallApplicationPathShortcut = QObject::tr("Uninstall %1").arg(_applicationName) + ".lnk";
    QString settingsApplicationPathShortcut = QObject::tr("Configure %1").arg(_applicationName) + ".lnk";

    QString loaderPath =  _installPath.getLoaderFilePath();
    QString description = _applicationName;
    QString executionDir = _installLocation;

    QString iconApplicationPath = _installPath.getImagesDirPath().absolutePath() + "/shortcut.ico";
    QString iconUninstallPath = _installPath.getImagesDirPath().absolutePath() + "/trash.ico";
    QString iconConfigurationPath = _installPath.getImagesDirPath().absolutePath() + "/config.ico";

    bool runApplicationShortcut = createShortcut(_installLocation + "/" + runApplicationPathShortcut, loaderPath, "", executionDir, iconApplicationPath, description);
    bool uninstallApplicationShortcut = createShortcut(_installLocation + "/" + uninstallApplicationPathShortcut, loaderPath, "-uninstall", executionDir, iconUninstallPath, description);
    bool configureApplicationShortcut = createShortcut(_installLocation + "/" + settingsApplicationPathShortcut, loaderPath, "-configure", executionDir, iconConfigurationPath, description);

    if (runApplicationShortcut && uninstallApplicationShortcut && configureApplicationShortcut) {
        L_INFO("Start copie shortcut in startmenu");
        QPair<bool, QString> copyApplicationShortcut = _installPath.extractResourceToPath(_installLocation + "/" + runApplicationPathShortcut, applicationFolder + "/" + runApplicationPathShortcut);
        QPair<bool, QString> copyUnstallationShortcut = _installPath.extractResourceToPath(_installLocation + "/" + uninstallApplicationPathShortcut, applicationFolder + "/" + uninstallApplicationPathShortcut);
        QPair<bool, QString> copyConfigurationShortcut = _installPath.extractResourceToPath(_installLocation + "/" + settingsApplicationPathShortcut, applicationFolder + "/" + settingsApplicationPathShortcut);

        if (copyApplicationShortcut.first && copyUnstallationShortcut.first && copyConfigurationShortcut.first) {
            L_INFO("Success Startmenu folder creation and shortcuts copie");
            return true;
        } else {
            // verirfy if shorcut has been moved in all user start menu programs
            folderPath = findAllUserStartMenuProgramsFolder();
            applicationFolder = folderPath + "/" + _startMenuFolderName;
            if (QDir(applicationFolder).exists()) {
                if (!QFile::exists(applicationFolder + "/" + runApplicationPathShortcut)) {
                    L_ERROR(copyApplicationShortcut.second);
                    return false;
                }
                if (!QFile::exists(applicationFolder + "/" + uninstallApplicationPathShortcut)) {
                    L_ERROR(copyUnstallationShortcut.second);
                    return false;
                }
                if (!QFile::exists(applicationFolder + "/" + settingsApplicationPathShortcut)) {
                    L_ERROR(copyConfigurationShortcut.second);
                    return false;
                }
                return true;
            }
            // shorcut doesn't exist, and no moved in all user start menu programs
            else {
                L_ERROR("Error when copy shortcut in startMenu folder");
                L_ERROR(copyApplicationShortcut.second);
                L_ERROR(copyUnstallationShortcut.second);
                L_ERROR(copyConfigurationShortcut.second);
                return false;
            }
        }
    } else {
        return false;
    }
}

bool WindowsShortcutImpl::createShortcut(QString shortcutPath, QString targetPath, QString _args, QString executionDir, QString iconPath, QString description)
{
    L_INFO("Shortcut creation : shortcutPath=" + shortcutPath + " targetPath=" + targetPath + " description=" + description + " executionDir=" + executionDir + " iconPath=" + iconPath + " args=" + _args);

    // remove file if exist
    if (QFile::exists(shortcutPath)) {
        L_INFO("Start remove existing shortcut " + shortcutPath);
        QFile f(shortcutPath);
        f.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        bool remove = f.remove();
        if (!remove || QFile::exists(shortcutPath)) {
            L_ERROR("Error when remove file" + shortcutPath);
            return false;
        }
    }

	std::string shortcutLatinChar = std::string(shortcutPath.toLatin1().constData());

    L_INFO("Starting shortcut creation ");
    HRESULT result = createWindowsShortcut((const wchar_t *)targetPath.utf16(), (const wchar_t *)_args.utf16(),
                                           shortcutLatinChar.c_str(), (const wchar_t *)description.utf16(),
                                           1, (const wchar_t *)executionDir.utf16(),
                                           (const wchar_t *)iconPath.utf16(), 0);
    if (SUCCEEDED(result)) {
        L_INFO("Success shortcut creation");
        if (!QFile::exists(shortcutPath)) {
            L_ERROR("Shortcut doesn't exist : " + shortcutPath);
            return false;
        }
    } else {
        L_ERROR("On error occured when shortcut creation");
        return false;
    }
    return true;
}

#ifndef _UNICODE
HRESULT WindowsShortcutImpl::createWindowsShortcut(LPCWSTR _pszTargetfile, LPCWSTR _pszTargetargs,
                                               const char * _pszLinkfile, LPCWSTR _pszDescription,
                                               int _iShowmode, LPCWSTR _pszCurdir,
                                               LPCWSTR _pszIconfile, int _iIconindex)
{
    // TODO manage accent in link file

    HRESULT hRes;                       /* Returned COM result code */
    IShellLink * pShellLink;            /* IShellLink object pointer */
    IPersistFile * pPersistFile;        /* IPersistFile object pointer */
    WCHAR wszLinkfile[MAX_PATH];        /* pszLinkfile as Unicode string */

    CoInitialize(NULL);
    hRes = E_INVALIDARG;
    if (
        (_pszTargetfile != NULL) && (lstrlen(_pszTargetfile) > 0) &&
        (_pszTargetargs != NULL) &&
        (_pszLinkfile != NULL) && (strlen(_pszLinkfile) > 0) &&
        (_pszDescription != NULL) &&
        (_iShowmode >= 0) &&
        (_pszCurdir != NULL) &&
        (_pszIconfile != NULL) &&
        (_iIconindex >= 0)
        ) {
        hRes = CoCreateInstance(
                CLSID_ShellLink,        /* pre-defined CLSID of the IShellLink object */
                NULL,                   /* pointer to parent interface if part of aggregate */
                CLSCTX_INPROC_SERVER,   /* caller and called code are in same process */
                IID_IShellLink,         /* pre-defined interface of the IShellLink object */
                (LPVOID *)&pShellLink); /* Returns a pointer to the IShellLink object */

        if (SUCCEEDED(hRes)) {
            /* Set the fields in the IShellLink object */
            hRes = pShellLink->SetPath(_pszTargetfile);
            hRes = pShellLink->SetArguments(_pszTargetargs);
            if (lstrlen(_pszDescription) > 0) {
                hRes = pShellLink->SetDescription(_pszDescription);
            }
            if (_iShowmode > 0) {
                hRes = pShellLink->SetShowCmd(_iShowmode);
            }
            if (lstrlen(_pszCurdir) > 0) {
                hRes = pShellLink->SetWorkingDirectory(_pszCurdir);
            }
            if (lstrlen(_pszIconfile) > 0 && _iIconindex >= 0) {
                hRes = pShellLink->SetIconLocation(_pszIconfile, _iIconindex);
            }

            /* Use the IPersistFile object to save the shell link */
            hRes = pShellLink->QueryInterface(
                    IID_IPersistFile,         /* pre-defined interface of the
                                               *  IPersistFile object */
                    (LPVOID *)&pPersistFile); /* returns a pointer to the
                                               * IPersistFile object */
            if (SUCCEEDED(hRes)) {
                MultiByteToWideChar(CP_ACP, 0,
                                    _pszLinkfile, -1,
                                    wszLinkfile, MAX_PATH);
                hRes = pPersistFile->Save(wszLinkfile, TRUE);
                pPersistFile->Release();
            }
            pShellLink->Release();
        }
    }
    CoUninitialize();
    return (hRes);
}
#endif

QString WindowsShortcutImpl::findAllUserStartMenuProgramsFolder()
{
    return findWindowsPath(CSIDL_COMMON_PROGRAMS);
}

QString WindowsShortcutImpl::findAllUserStartupFolder()
{
    return findWindowsPath(CSIDL_COMMON_STARTUP);
}

QString WindowsShortcutImpl::findUserStartMenuProgramFolder()
{
    return findWindowsPath(CSIDL_PROGRAMS);
}

QString WindowsShortcutImpl::findUserStartupFolder()
{
    return findWindowsPath(CSIDL_STARTUP);
}

QString WindowsShortcutImpl::findWindowsPath(int hToken)
{
    WCHAR path[MAX_PATH];
    HRESULT hr = SHGetFolderPathW(NULL, hToken, NULL, 0, path);

    if (SUCCEEDED(hr)) {
        QString p = QString::fromWCharArray(path);
        return p;
    } else {
        L_INFO("Impossible to find windows path with handle = " + QString::number(hToken));
    }
    return "";
}

