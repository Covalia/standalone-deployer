#include "windowsshortcutmanager.h"

#include "settings/settings.h"
#include "log/logger.h"
#include "shortcut/shortcut.h"
#include "fs/apptreemanager.h"

#include <QStandardPaths>

WindowsShortcutManager::WindowsShortcutManager()
{
}

bool WindowsShortcutManager::createDesktopShortcut(QString _shortcutName, QString _args)
{
    L_INFO("Start preparation of desktop shortcut");
    Settings * settings = Settings::getInstance();
    AppTreeManager * treeManager = new AppTreeManager(QDir(settings->getInstallLocation()));
    QString loaderPath =  treeManager->getLoaderFilePath();
    QString shortcutPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + _shortcutName + ".lnk";
    QString description = settings->getApplicationName();
    QString executionDir = settings->getInstallLocation();
    QString iconPath = treeManager->getImagesDirPath().absolutePath() + "/shortcut.ico";
    return createShortcut(shortcutPath, loaderPath, _args, executionDir, iconPath, description);
}

bool WindowsShortcutManager::createStartShorcut(QString _shortcutName, bool _allUser)
{
    L_INFO("Start creation shortcut in startup folder to run at computer start");
    Shortcut * shorcutCreator = new Shortcut();
    Settings * settings = Settings::getInstance();
    AppTreeManager * treeManager = new AppTreeManager(QDir(settings->getInstallLocation()));
    QString loaderPath =  treeManager->getLoaderFilePath();
    QString description = settings->getApplicationName();
    QString executionDir = settings->getInstallLocation();
    QString iconPath = treeManager->getImagesDirPath().absolutePath() + "/shortcut.ico";
    if (_allUser) {
        return createShortcut(shorcutCreator->findAllUserStartupFolder() + "/" + _shortcutName + ".lnk", loaderPath, "", executionDir, iconPath, description);
    } else {
        return createShortcut(shorcutCreator->findUserStartupFolder() + "/" + _shortcutName + ".lnk", loaderPath, "", executionDir, iconPath, description);
    }
}

bool WindowsShortcutManager::createStartMenuShorcut(QString _startMenuFolderName, bool _allUser)
{
    L_INFO("Start creation shortcut in startMenu folder");
    Settings * settings = Settings::getInstance();
    AppTreeManager * treeManager = new AppTreeManager(QDir(settings->getInstallLocation()));
    Shortcut * shorcutCreator = new Shortcut();
    QString folderPath("");

    if (_allUser) {
        folderPath = shorcutCreator->findAllUserStartMenuProgramsFolder();
    } else {
        folderPath = shorcutCreator->findUserStartMenuProgramFolder();
    }

    // startMenu folder creation
    QString applicationFolder = folderPath + "/" + _startMenuFolderName;
    QDir applicationFolderDir(applicationFolder);
    if (applicationFolderDir.exists()) {
        L_INFO("Remove folder " + applicationFolder);
        bool successRemoveDir = applicationFolderDir.removeRecursively();
        if (!successRemoveDir) {
            L_ERROR("An error occured when removing folder " + applicationFolder);
            return false;
        }
    }

    bool successCreationStartMenuFolder = AppTreeManager::makeDirectoryIfNotExists(folderPath, _startMenuFolderName);

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

    QString runApplicationPathShortcut =  settings->getApplicationName() + ".lnk";
    QString uninstallApplicationPathShortcut = QObject::tr("Uninstall %1").arg(settings->getApplicationName()) + ".lnk";
    QString settingsApplicationPathShortcut = QObject::tr("Configure %1").arg(settings->getApplicationName()) + ".lnk";

    QString loaderPath =  treeManager->getLoaderFilePath();
    QString description = settings->getApplicationName();
    QString executionDir = settings->getInstallLocation();

    QString iconApplicationPath = treeManager->getImagesDirPath().absolutePath() + "/shortcut.ico";
    QString iconUninstallPath = treeManager->getImagesDirPath().absolutePath() + "/trash.ico";
    QString iconConfigurationPath = treeManager->getImagesDirPath().absolutePath() + "/config.ico";

    bool runApplicationShortcut = createShortcut(settings->getInstallLocation() + "/" + runApplicationPathShortcut, loaderPath, "", executionDir, iconApplicationPath, description);
    bool uninstallApplicationShortcut = createShortcut(settings->getInstallLocation() + "/" + uninstallApplicationPathShortcut, loaderPath, "-uninstall", executionDir, iconUninstallPath, description);
    bool configureApplicationShortcut = createShortcut(settings->getInstallLocation() + "/" + settingsApplicationPathShortcut, loaderPath, "-configure", executionDir, iconConfigurationPath, description);

    if (runApplicationShortcut && uninstallApplicationShortcut && configureApplicationShortcut) {
        L_INFO("Start copie shortcut in startmenu");
        QPair<bool, QString> copyApplicationShortcut = treeManager->extractResourceToPath(settings->getInstallLocation() + "/" + runApplicationPathShortcut, applicationFolder + "/" + runApplicationPathShortcut);
        QPair<bool, QString> copyUnstallationShortcut = treeManager->extractResourceToPath(settings->getInstallLocation() + "/" + uninstallApplicationPathShortcut, applicationFolder + "/" + uninstallApplicationPathShortcut);
        QPair<bool, QString> copyConfigurationShortcut = treeManager->extractResourceToPath(settings->getInstallLocation() + "/" + settingsApplicationPathShortcut, applicationFolder + "/" + settingsApplicationPathShortcut);

        if (copyApplicationShortcut.first && copyUnstallationShortcut.first && copyConfigurationShortcut.first) {
            L_INFO("Success Startmenu folder creation and shortcuts copie");
            return true;
        } else {
            L_ERROR("Error when copy shortcut in startMenu folder");
            L_ERROR(copyApplicationShortcut.second);
            L_ERROR(copyUnstallationShortcut.second);
            L_ERROR(copyConfigurationShortcut.second);
            return false;
        }
    } else {
        return false;
    }
}

bool WindowsShortcutManager::createShortcut(QString shortcutPath, QString targetPath, QString _args, QString executionDir, QString iconPath, QString description)
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

    #ifdef _WIN32
        std::string strLoaderPath = shortcutPath.toStdString();
        L_INFO("Starting shortcut creation ");
        Shortcut * shorcutCreator = new Shortcut();
        HRESULT result = shorcutCreator->createWindowsShortcut((const wchar_t *)targetPath.utf16(), (const wchar_t *)_args.utf16(),
                                                               const_cast<char *>(strLoaderPath.c_str()), (const wchar_t *)description.utf16(),
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
    #endif
    return true;
}
