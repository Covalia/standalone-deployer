#include "installmanager.h"

#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QProcess>

#include "log/logger.h"
#include "shortcut/shortcut.h"
#include "commandline/commandlineparser.h"
#include "settings/settings.h"
#include "settings/resourcessettings.h"
#include "lang/languagemanager.h"
#include "gui/style/stylemanager.h"
#include "fs/apptreemanager.h"
#include "fs/config.h"
#include "fs/resourcesutil.h"
#include "windowsshortcutmanager.h"

InstallManager::InstallManager() : QObject(), m_uiManager(0)
{
}

InstallManager::~InstallManager()
{
    delete m_uiManager;
}

void InstallManager::initInstallation()
{
    // init project resources
    ResourcesSettings * projectSettings = ResourcesSettings::getInstance();

    projectSettings->initSettings(":/resources/project.ini");
    projectSettings->readSettings();
    projectSettings->sendToSettings();
    L_INFO("Ip=" + projectSettings->getIp_server());

    L_INFO("Parsing command line");
    CommandLineParser * lineParser = new CommandLineParser();
    runAppAfter = lineParser->isRunApp();
    L_INFO("Run after = " + QString::number(runAppAfter));
    lineParser->sendToSettings();

    // init style
    StyleManager::setGeneralStyle();

    LanguageManager::initLanguage();

    if (!lineParser->isSilent()) {
        qApp->setWindowIcon(QIcon(":/images/icon.png"));
        qApp->setApplicationName(QString(QObject::tr("Standalone deployment")));

        m_uiManager = new UIManager();
        m_uiManager->init();

        QObject::connect(m_uiManager, SIGNAL(changeInstallationSignal()),
                         this, SLOT(eventStartInstallation()));
    } else {
        startInstallation();
    }
}

void InstallManager::startInstallation()
{
    Settings * settings = Settings::getInstance();

    L_INFO("End of parameters initialization");
    L_INFO("Setings before start installation  : \n********\n" + settings->paramListString() + "********\n");

    QString errorMessage = "";

    // tree creation
    bool folderCreation = createInstallationFolders(settings->getInstallLocation());
    if (!folderCreation) {
        errorMessage = tr("An error ocurred during folder creation");
    }

    // settings writing
    bool settingWriting =  createIniConfigurationFile();
    if (!settingWriting) {
        errorMessage = tr("An error ocurred during parameters writing");
    }

    // extract resources
    bool succesExtractResources =  extractResources();
    if (!succesExtractResources) {
        errorMessage = tr("An error ocurred during resources extraction");
    }

    // create shorcut
    bool succesCreateShortcut = createDesktopShortcut();
    if (!succesCreateShortcut) {
        errorMessage = tr("An error ocurred during shortcut creation");
    }

    bool success = folderCreation && settingWriting && succesExtractResources;

    if (m_uiManager) {
        if (success) {
            L_INFO("Success Installation");
            m_uiManager->eventEndInstallation(success, "");
        } else {
            m_uiManager->eventEndInstallation(success, errorMessage);
        }

        QObject::connect(m_uiManager, SIGNAL(closeInstallationSignal(bool)),
                         this, SLOT(eventCloseInstallation(bool)));
    } else {
        closeInstallation(runAppAfter);
    }
}

bool InstallManager::createInstallationFolders(QString installDir)
{
    L_INFO("Start the installation in directory : " + installDir);
    AppTreeManager * treeManager = new AppTreeManager(QDir(installDir));

    if (!treeManager->createDirectoryIfNotExist()) {
        L_ERROR("Error when create installation folder ");
        return false;
    } else {
        L_INFO("Succes of installation folder verification ");

        if (!treeManager->makeAppDirectories()) {
            L_ERROR("Error when create sub-installation folder (application tree) ");
            return false;
        }

        L_INFO("Succes of sub-installation folder creation ");
        return true;
    }
}

bool InstallManager::createIniConfigurationFile()
{
    Settings * settings = Settings::getInstance();

    QString installPath(settings->getInstallLocation() + "/" + FileSystemConfig::ConfigurationDir);
    QString installFile("standalone-deployer.ini");
    QString installFilePath(installPath + "/" + installFile);

    L_INFO("Start to create param file. Path = " + installFilePath);
    settings->initSettings(installFilePath);
    settings->writeSettings();

    if (QDir(installPath).exists(installFile)) {
        return true;
    } else {
        L_ERROR("An error occurred on the configuration file " + installPath + ". File is not created. ");
        return false;
    }
}

bool InstallManager::extractResources()
{
    Settings * settings = Settings::getInstance();

    QPair<bool, QString> extractUpdater = ResourcesUtil::extractResourceToPath(ResourcesUtil::getUpdaterResourcesPath(), ResourcesUtil::getUpdaterPath(settings->getInstallLocation()));
    L_INFO(extractUpdater.second);

    QPair<bool, QString> extractLoader = ResourcesUtil::extractResourceToPath(ResourcesUtil::getLoaderResourcesPath(), ResourcesUtil::getLoaderPath(settings->getInstallLocation()));
    L_INFO(extractLoader.second);

    QPair<bool, QString> extractAppIcon = ResourcesUtil::extractResourceToPath(":/images/shortcut.ico", settings->getInstallLocation() + "/shortcut.ico");
    L_INFO(extractAppIcon.second);

    return extractUpdater.first && extractLoader.first && extractAppIcon.first;
}

bool InstallManager::createDesktopShortcut()
{
    Settings * settings = Settings::getInstance();

    #ifdef _WIN32
        // WINDOWS SHORCUT
        // - online desktop shortcut
        // - offline desktop shorcut
        // - startMenu

        WindowsShortcutManager * windowsShortcutManager = new WindowsShortcutManager();
        bool success = true;
        // online shortcut
        if (settings->getShortcutOnline()) {
            success = success && windowsShortcutManager->createDesktopShortcut(settings->getShortcutName(), "");
        }
        // offline shortcut
        if (settings->getShortcutOffline()) {
            success = success && windowsShortcutManager->createDesktopShortcut(settings->getShortcutOfflineName(), settings->getShortcutOfflineArgs());
        }
        return success;

    #elif TARGET_OS_MAC
        // MAC OS SHORTCUT
        return true;
    #endif
}

bool InstallManager::launchLoader()
{
    Settings * settings = Settings::getInstance();
    QProcess process;
    QString loaderFile = ResourcesUtil::getLoaderPath(settings->getInstallLocation());

    if (!QFile::exists(loaderFile)) {
        L_ERROR("An error occured when launch file " + loaderFile + ". The file doesn't exist.");
        return false;
    }
    L_INFO("Launch file " + loaderFile);
    bool success = process.startDetached(loaderFile);
    if (!success) {
        L_ERROR("Error when launching file " + loaderFile);
    } else {
        L_INFO("Success launching file " + loaderFile);
    }
    return success;
}

void InstallManager::closeInstallation(bool launchApplication)
{
    if (launchApplication) {
        launchLoader();
    }
    L_INFO("End treatment, close application");
    qApp->exit();
}

void InstallManager::eventStartInstallation()
{
    startInstallation();
}

void InstallManager::eventCloseInstallation(bool launchApplication)
{
    closeInstallation(launchApplication);
}
