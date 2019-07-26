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
    L_INFO("Will create desktop shortcut");
    QSharedPointer<QFile> loaderFile = _appPath.getLoaderFile();
    QFile shortcutFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + QDir::separator() + _shortcutName + ".lnk");
    const QDir installDir = _appPath.getInstallationDir();
    const QFile iconFile(_appPath.getImagesDir().absoluteFilePath("shortcut.ico"));
    return createShortcut(shortcutFile, *loaderFile, _args, installDir, iconFile, _applicationName);
}

bool WindowsShortcutImpl::createStartShorcut(AppPath _appPath, QString _shortcutName, QString _applicationName)
{
    L_INFO("Will create shortcut in startup folder");
    QSharedPointer<QFile> loaderFile = _appPath.getLoaderFile();
    const QDir installDir = _appPath.getInstallationDir();
    const QFile iconFile(_appPath.getImagesDir().absoluteFilePath("shortcut.ico"));

    QFile file(findUserStartupFolder() + QDir::separator() + _shortcutName + ".lnk");
    return createShortcut(file, *loaderFile, "", installDir, iconFile, _applicationName);
}

bool WindowsShortcutImpl::createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName)
{
    L_INFO("Will create shortcut in startMenu folder");
    QString folderPath("");

    folderPath = findUserStartMenuProgramFolder();

    // startMenu folder creation
    QString applicationFolder = folderPath + QDir::separator() + _startMenuFolderName;

    bool successCreationStartMenuFolder = _appPath.makeDirectoryIfNotExisting(QDir(folderPath + QDir::separator() + _startMenuFolderName));

    if (!successCreationStartMenuFolder) {
        L_ERROR(QString("Startmenu folder does not exist: %1. Startmenu installation failed.").arg(applicationFolder));
        return false;
    } else {
        L_INFO(QString("Startmenu folder created: %1").arg(applicationFolder));
    }

    QFileInfo applicationFolderInfo(applicationFolder);
    if (!applicationFolderInfo.isDir()) {
        L_ERROR(QString("Startmenu folder is not a directory: %1. Startmenu installation failed.").arg(applicationFolder));
        return false;
    }
    if (!applicationFolderInfo.isWritable()) {
        L_ERROR(QString("Startmenu folder is not writable: %1. Startmenu installation failed.").arg(applicationFolder));
        return false;
    }

    const QString runApplicationPathShortcut =  _applicationName + ".lnk";
    const QString settingsApplicationPathShortcut = QObject::tr("Configuration %1").arg(_applicationName) + ".lnk";

    QSharedPointer<QFile> loader = _appPath.getLoaderFile();
    const QString description = _applicationName;
    const QDir installDir = _appPath.getInstallationDir();

    const QString iconApplicationPath = _appPath.getImagesDir().absoluteFilePath("shortcut.ico");
    const QString iconConfigurationPath = _appPath.getImagesDir().absoluteFilePath("config.ico");

    QFile appShortcutIn(installDir.absoluteFilePath(runApplicationPathShortcut));
    QFile configShortcutIn(installDir.absoluteFilePath(settingsApplicationPathShortcut));

    bool runApplicationShortcut = createShortcut(appShortcutIn, *loader, "", installDir, iconApplicationPath, description);
    bool configureApplicationShortcut = createShortcut(configShortcutIn, *loader, "--configure", installDir, iconConfigurationPath, description);

    if (runApplicationShortcut && configureApplicationShortcut) {
        L_INFO("Will copy shortcut into startmenu");

        QFile appShortcutOut(applicationFolder + QDir::separator() + runApplicationPathShortcut);
        const QPair<bool, QString> copyApplicationShortcut = _appPath.extractResource(appShortcutIn, appShortcutOut);

        QFile configShortcutOut(applicationFolder + QDir::separator() + settingsApplicationPathShortcut);
        const QPair<bool, QString> copyConfigurationShortcut = _appPath.extractResource(configShortcutIn, configShortcutOut);

        if (copyApplicationShortcut.first && copyConfigurationShortcut.first) {
            L_INFO("Startmenu folder created and shortcuts copied");
            return true;
        }
    } else {
        return false;
    }
}

bool WindowsShortcutImpl::createShortcut(QFile &_shortcutFile, const QFile &_targetFile, QString _args, QDir _workingDir, const QFile &_iconFile, QString _description)
{
    L_INFO(QString("Shortcut creation - shortcutFile: %1, targetFile: %2, description: %3, workingDir: %4, iconFile: %5, args: %6")
           .arg(_shortcutFile.fileName())
           .arg(_targetFile.fileName())
           .arg(_description)
           .arg(_workingDir.absolutePath())
           .arg(_iconFile.fileName())
           .arg(_args));

    // remove file if exist
    if (_shortcutFile.exists()) {
        L_INFO(QString("Removing existing shortcut: %1").arg(_shortcutFile.fileName()));
        _shortcutFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
        bool remove = _shortcutFile.remove();
        if (!remove || _shortcutFile.exists()) {
            L_ERROR(QString("Unable to remove file: %1").arg(_shortcutFile.fileName()));
            return false;
        } else {
            L_INFO(QString("File removed: %1").arg(_shortcutFile.fileName()));
        }
    }

    L_INFO("Will create shortcut");
    return createWindowsShortcut(_targetFile.fileName(), _args, _shortcutFile.fileName(), _description,
                                 _workingDir.absolutePath(), _iconFile.fileName(), 0);
}

LPCWSTR WindowsShortcutImpl::lpcwstrFromQString(QString _qstring)
{
    wchar_t * ch = new wchar_t[_qstring.length() + 1];

    _qstring.toWCharArray(ch);
    ch[_qstring.length()] = 0;
    return ch;
}

HRESULT WindowsShortcutImpl::createWindowsShortcut(QString _targetFile, QString _targetArgs,
                                                   QString _linkFile, QString _description,
                                                   QString _currentDir, QString _iconFile, int _iconIndex)
{
    if (_targetFile.isEmpty() || _linkFile.isEmpty()) {
        L_WARN("Target file or link file are empty. Aborting.");
        return false;
    }

    bool result = true;

    // init com library
    CoInitialize(0);

    IShellLink * psl;

    LPCWSTR w_targetFile = lpcwstrFromQString(_targetFile);
    LPCWSTR w_targetArgs = lpcwstrFromQString(_targetArgs);
    LPCWSTR w_description = lpcwstrFromQString(_description);
    LPCWSTR w_currentDir = lpcwstrFromQString(_currentDir);
    LPCWSTR w_iconFile = lpcwstrFromQString(_iconFile);
    LPCWSTR w_linkFile = lpcwstrFromQString(_linkFile);

    HRESULT hres = CoCreateInstance(CLSID_ShellLink, 0, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&psl);
    if (!SUCCEEDED(hres)) {
        L_ERROR("Error while creating IShellLink instance");
        result = false;
    } else {
        hres = psl->SetPath(w_targetFile);
        if (!SUCCEEDED(hres)) {
            L_ERROR("Error while setting path");
            result = false;
        } else {
            hres = psl->SetShowCmd(1);
            if (!SUCCEEDED(hres)) {
                L_ERROR("Error while setting show cmd");
                result = false;
            } else {
                if (!_targetArgs.isEmpty()) {
                    hres = psl->SetArguments(w_targetArgs);
                }
                if (!SUCCEEDED(hres)) {
                    L_ERROR("Error while setting arguments");
                    result = false;
                } else {
                    if (!_description.isEmpty()) {
                        hres = psl->SetDescription(w_description);
                    }
                    if (!SUCCEEDED(hres)) {
                        L_ERROR("Error while setting description");
                        result = false;
                    } else {
                        if (!_currentDir.isEmpty()) {
                            hres = psl->SetWorkingDirectory(w_currentDir);
                        }
                        if (!SUCCEEDED(hres)) {
                            L_ERROR("Error while setting working directory");
                            result = false;
                        } else {
                            if (!_iconFile.isEmpty() && _iconIndex >= 0) {
                                hres = psl->SetIconLocation(w_iconFile, _iconIndex);
                            }
                            if (!SUCCEEDED(hres)) {
                                L_ERROR("Error while setting icon location");
                                result = false;
                            } else {
                                IPersistFile * ppf;
                                // get IPersistFile interface
                                hres = psl->QueryInterface(IID_IPersistFile, (LPVOID *)&ppf);
                                if (!SUCCEEDED(hres)) {
                                    L_ERROR("Error while setting query interface");
                                    result = false;
                                } else {
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
        return QString::fromWCharArray(path);
    } else {
        L_WARN(QString("Unable to find windows path for key: %1").arg(QString::number(hToken)));
    }
    return "";
}
