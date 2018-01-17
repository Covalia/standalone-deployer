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
    QString shortcutFolderPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    return createShortcut(shortcutFolderPath, _shortcutName, _args);
}

bool WindowsShortcutManager::createStartShorcut(QString _shortcutName, bool allUser)
{
    Shortcut * shorcutCreator = new Shortcut();
    QString folderPath("");

    if (allUser) {
        folderPath = shorcutCreator->findAllUserStartupFolder();
    } else {
        folderPath = shorcutCreator->findUserStartupFolder();
    }
    L_INFO("Start creation shortcut in start menu folder to run at computer start");
    return createShortcut(folderPath, _shortcutName, "");
}

bool WindowsShortcutManager::createShortcut(QString _shortcutFolderPath, QString _shortcutName, QString _args)
{
    Settings * settings = Settings::getInstance();
    AppTreeManager * m_treeManager = new AppTreeManager(QDir(settings->getInstallLocation()));

    QString loaderPath =  m_treeManager->getLoaderFilePath();
    QString shortcutPath = _shortcutFolderPath + "/" + _shortcutName + ".lnk";
    QString description = settings->getApplicationName();
    QString executionDir = settings->getInstallLocation();
    QString iconPath = settings->getInstallLocation() + "/shortcut.ico";

    L_INFO("Shortcut creation : shortcutPath=" + shortcutPath + " loaderPath=" + loaderPath + " description=" + description + " executionDir=" + executionDir + " iconPath=" + iconPath + " args=" + _args);

    #ifdef _WIN32
        std::string strLoaderPath = shortcutPath.toStdString();

        L_INFO("Starting shortcut creation ");

        Shortcut * shorcutCreator = new Shortcut();

        HRESULT result = shorcutCreator->createWindowsShortcut((const wchar_t *)loaderPath.utf16(), (const wchar_t *)_args.utf16(),
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
