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

    L_INFO("Starting shortcut creation ");
    return createWindowsShortcut(targetPath, _args, shortcutPath, description,
                                           executionDir, iconPath, 0);
}

LPCWSTR WindowsShortcutImpl::lpcwstrFromQString(QString _qstring) {
    wchar_t* ch = new wchar_t[_qstring.length() + 1];
    _qstring.toWCharArray(ch);
    ch[_qstring.length()] = 0;
    return ch;
}


HRESULT WindowsShortcutImpl::createWindowsShortcut(QString _targetFile, QString _targetArgs,
                                               QString _linkFile, QString _description,
                                               QString _currentDir, QString _iconFile, int _iconIndex)
{
    if (_targetFile.isEmpty() || _linkFile.isEmpty()) {
       L_INFO("target file or link file is empty");
       return false;
    }

    bool result = true;

    // init com library
    CoInitialize(0);

    IShellLink *psl;

    LPCWSTR w_targetFile = lpcwstrFromQString(_targetFile);
    LPCWSTR w_targetArgs = lpcwstrFromQString(_targetArgs);
    LPCWSTR w_description = lpcwstrFromQString(_description);
    LPCWSTR w_currentDir = lpcwstrFromQString(_currentDir);
    LPCWSTR w_iconFile = lpcwstrFromQString(_iconFile);
    LPCWSTR w_linkFile = lpcwstrFromQString(_linkFile);

    HRESULT hres = CoCreateInstance(CLSID_ShellLink, 0, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *) &psl);
    if (!SUCCEEDED(hres)) {
        L_ERROR("Error while creating IShellLink instance");
        result = false;
    }
    else {

        hres = psl->SetPath(w_targetFile);
        if (!SUCCEEDED(hres)) {
            L_ERROR("Error while setting path");
            result = false;
        }
        else {
            hres = psl->SetShowCmd(1);
            if (!SUCCEEDED(hres)) {
                L_ERROR("Error while setting show cmd");
                result = false;
            }
            else {
                if (!_targetArgs.isEmpty()) {
                    hres = psl->SetArguments(w_targetArgs);
                }
                if (!SUCCEEDED(hres)) {
                    L_ERROR("Error while setting arguments");
                    result = false;
                }
                else {
                    if (!_description.isEmpty()) {
                        hres = psl->SetDescription(w_description);
                    }
                    if (!SUCCEEDED(hres)) {
                        L_ERROR("Error while setting description");
                        result = false;
                    }
                    else {
                        if (!_currentDir.isEmpty()) {
                            hres = psl->SetWorkingDirectory(w_currentDir);
                        }
                        if (!SUCCEEDED(hres)) {
                            L_ERROR("Error while setting working directory");
                            result = false;
                        }
                        else {
                            if (!_iconFile.isEmpty() && _iconIndex >= 0) {
                                hres = psl->SetIconLocation(w_iconFile, _iconIndex);
                            }
                            if (!SUCCEEDED(hres)) {
                                L_ERROR("Error while setting icon location");
                                result = false;
                            }
                            else {
                                IPersistFile *ppf;
                                // get IPersistFile interface
                                hres = psl->QueryInterface(IID_IPersistFile, (LPVOID *) &ppf);
                                if (!SUCCEEDED(hres)) {
                                    L_ERROR("Error while setting query interface");
                                    result = false;
                                }
                                else {
                                    hres = ppf->Save(w_linkFile, 1);
                                    if (hres != S_OK) {
                                        L_ERROR("Error while saving shortcut");
                                        result = false;
                                    }
                                }
                                ppf->Release();
                            }
                        }
                    }
                }
            }
        }
        psl->Release();
    }

	delete[] w_targetFile;
	delete[] w_targetArgs;
	delete[] w_description;
	delete[] w_currentDir;
	delete[] w_iconFile;
	delete[] w_linkFile;

    CoUninitialize();

    return result;
}

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

