#include "factories/shortcut/windows/windowsshortcutimpl.h"

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

bool WindowsShortcutImpl::createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName)
{
    L_INFO("Start preparation of desktop shortcut");
    QSharedPointer<QFile> loaderFile = _appPath.getLoaderFile();
    QFile shortcutFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QDir::separator() + _shortcutName + ".lnk");
    QDir installDir = _appPath.getInstallationDir();
    QFile iconFile(_appPath.getImagesDir().absoluteFilePath("shortcut.ico"));
    return createShortcut(shortcutFile, *loaderFile, _args, installDir, iconFile, _applicationName);
}

bool WindowsShortcutImpl::createStartShorcut(AppPath _appPath, QString _shortcutName, bool _allUser, QString _applicationName)
{
    L_INFO("Start creation shortcut in startup folder to run at computer start");
    QSharedPointer<QFile> loaderFile = _appPath.getLoaderFile();
    QDir installDir = _appPath.getInstallationDir();
    QFile iconFile(_appPath.getImagesDir().absoluteFilePath("shortcut.ico"));
    if (_allUser) {
        QFile file(findAllUserStartupFolder() + QDir::separator() + _shortcutName + ".lnk");
        return createShortcut(file, *loaderFile, "", installDir, iconFile, _applicationName);
    } else {
        QFile file(findUserStartupFolder() + QDir::separator() + _shortcutName + ".lnk");
        return createShortcut(file, *loaderFile, "", installDir, iconFile, _applicationName);
    }
}

bool WindowsShortcutImpl::createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, bool _allUser, QString _applicationName)
{
    L_INFO("Start creation shortcut in startMenu folder");
    QString folderPath("");

    if (_allUser) {
        folderPath = findAllUserStartMenuProgramsFolder();
    } else {
        folderPath = findUserStartMenuProgramFolder();
    }

    // startMenu folder creation
    QString applicationFolder = folderPath + QDir::separator() + _startMenuFolderName;

    bool successCreationStartMenuFolder = _appPath.makeDirectoryIfNotExisting(QDir(folderPath + QDir::separator() + _startMenuFolderName));

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
    QString settingsApplicationPathShortcut = QObject::tr("Configure %1").arg(_applicationName) + ".lnk";

    QSharedPointer<QFile> loader = _appPath.getLoaderFile();
    QString description = _applicationName;
    QDir installDir = _appPath.getInstallationDir();

    QString iconApplicationPath = _appPath.getImagesDir().absoluteFilePath("shortcut.ico");
    QString iconConfigurationPath = _appPath.getImagesDir().absoluteFilePath("config.ico");

    QFile appShortcutIn(installDir.absoluteFilePath(runApplicationPathShortcut));
    QFile configShortcutIn(installDir.absoluteFilePath(settingsApplicationPathShortcut));

    bool runApplicationShortcut = createShortcut(appShortcutIn, *loader, "", installDir, iconApplicationPath, description);
    bool configureApplicationShortcut = createShortcut(configShortcutIn, *loader, "-configure", installDir, iconConfigurationPath, description);

    if (runApplicationShortcut && configureApplicationShortcut) {
        L_INFO("Start copie shortcut in startmenu");

        QFile appShortcutOut(applicationFolder + QDir::separator() + runApplicationPathShortcut);
        QPair<bool, QString> copyApplicationShortcut = _appPath.extractResource(appShortcutIn, appShortcutOut);

        QFile configShortcutOut(applicationFolder + QDir::separator() + settingsApplicationPathShortcut);
        QPair<bool, QString> copyConfigurationShortcut = _appPath.extractResource(configShortcutIn, configShortcutOut);

        if (copyApplicationShortcut.first && copyConfigurationShortcut.first) {
            L_INFO("Success Startmenu folder creation and shortcuts copie");
            return true;
        } else {
            // verirfy if shorcut has been moved in all user start menu programs
            folderPath = findAllUserStartMenuProgramsFolder();
            applicationFolder = folderPath + QDir::separator() + _startMenuFolderName;
            if (QDir(applicationFolder).exists()) {
                if (!QFile::exists(applicationFolder + QDir::separator() + runApplicationPathShortcut)) {
                    L_ERROR(copyApplicationShortcut.second);
                    return false;
                }
                if (!QFile::exists(applicationFolder + QDir::separator() + settingsApplicationPathShortcut)) {
                    L_ERROR(copyConfigurationShortcut.second);
                    return false;
                }
                return true;
            }
            // shorcut doesn't exist, and no moved in all user start menu programs
            else {
                L_ERROR("Error when copy shortcut in startMenu folder");
                L_ERROR(copyApplicationShortcut.second);
                L_ERROR(copyConfigurationShortcut.second);
                return false;
            }
        }
    } else {
        return false;
    }
}

bool WindowsShortcutImpl::createShortcut(QFile &_shortcutFile, const QFile &_targetFile, QString _args, QDir _workingDir, const QFile &_iconFile, QString _description)
{
    L_INFO("Shortcut creation : shortcutFile=" + _shortcutFile.fileName() + " targetFile=" + _targetFile.fileName() + " description=" + _description + " workingDir=" + _workingDir.absolutePath() + " iconFile=" + _iconFile.fileName() + " args=" + _args);

    // remove file if exist
    if (_shortcutFile.exists()) {
        L_INFO("Start remove existing shortcut " + _shortcutFile.fileName());
        _shortcutFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        bool remove = _shortcutFile.remove();
        if (!remove || _shortcutFile.exists()) {
            L_ERROR("Error when remove file" + _shortcutFile.fileName());
            return false;
        }
    }

    L_INFO("Starting shortcut creation ");
    return createWindowsShortcut(_targetFile.fileName(), _args, _shortcutFile.fileName(), _description,
                                           _workingDir.absolutePath(), _iconFile.fileName(), 0);
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

