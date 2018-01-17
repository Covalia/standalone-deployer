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
    Settings * settings = Settings::getInstance();
    AppTreeManager * m_treeManager = new AppTreeManager(QDir(settings->getInstallLocation()));

    L_INFO("Start preparation of desktop shortcut");

    QString loaderPath =  m_treeManager->getLoaderFilePath();
    QString shortcutPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + _shortcutName + ".lnk";
    QString description = settings->getApplicationName();
    QString executionDir = settings->getInstallLocation();
    QString iconPath = settings->getInstallLocation() + "/shortcut.ico";

    L_INFO("Shortcut shortcutPath " + shortcutPath);
    L_INFO("Shortcut loaderPath " + loaderPath);
    L_INFO("Shortcut description " + description);
    L_INFO("Shortcut executionDir " + executionDir);
    L_INFO("Shortcut iconPath " + iconPath);
    L_INFO("Shortcut args " + _args);

    #ifdef _WIN32
        Shortcut * shorcutCreator = new Shortcut();

        std::string strLoaderPath = shortcutPath.toStdString();

        L_INFO("Starting shortcut creation ");

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

        QString allUserStartMenu = shorcutCreator->findAllUserStartMenuFolder();
        QString userStartMenu = shorcutCreator->findUserStartMenuFolder();
        L_INFO(userStartMenu);

    #endif

    return true;
}
