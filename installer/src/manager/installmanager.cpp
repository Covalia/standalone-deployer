#include "installmanager.h"

#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QProcess>
#include <QThread>

#include "log/logger.h"
#include "shortcut/shortcut.h"
#include "commandline/commandlineparser.h"
#include "settings/settings.h"
#include "settings/resourcessettings.h"
#include "lang/languagemanager.h"
#include "gui/style/stylemanager.h"
#include "fs/config.h"
#include "windowsshortcutmanager.h"

InstallManager::InstallManager() : QObject(),
    m_uiManager(0),
    m_treeManager(0)
{
}

InstallManager::~InstallManager()
{
    delete m_uiManager;
    delete m_treeManager;
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

    Settings * settings = Settings::getInstance();
    m_treeManager = new AppTreeManager(QDir(settings->getInstallLocation()));

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
        QObject::connect(this, SIGNAL(endInstallation(bool,QString)),
                         m_uiManager, SLOT(eventEndInstallation(bool,QString)));
    } else {
        startInstallation();
    }
}

void InstallManager::eventStartInstallation()
{
    startInstallation();
}

void InstallManager::startInstallation()
{
    Settings * settings = Settings::getInstance();

    L_INFO("End of parameters initialization");
    L_INFO("Settings before start installation  : \n********\n" + settings->paramListString() + "********\n");

    QString errorMessage = "";

    // tree creation
    bool folderCreation = createInstallationFolders();
    if (!folderCreation) {
        errorMessage = tr("An error ocurred during folder creation");
    }

    // settings writing
    bool settingWriting =  createIniConfigurationFile();
    if (!settingWriting) {
        errorMessage = tr("An error ocurred during parameters writing");
    }

    QThread::sleep(1);

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
            emit endInstallation(success, "");
        } else {
            emit endInstallation(success, errorMessage);
        }

        QObject::connect(m_uiManager, SIGNAL(closeInstallationSignal(bool)),
                         this, SLOT(eventCloseInstallation(bool)));
    } else {
        closeInstallation(runAppAfter);
    }
}

bool InstallManager::createInstallationFolders()
{
    L_INFO("Start the installation in directory : ");

    if (!m_treeManager->createDirectoryIfNotExist()) {
        L_ERROR("Error when create installation folder ");
        return false;
    } else {
        L_INFO("Succes of installation folder verification ");

        if (!m_treeManager->makeAppDirectories()) {
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

    QString installFilePath(m_treeManager->getConfigurationFilePath());

    L_INFO("Start to create param file. Path = " + installFilePath);
    settings->initSettings(installFilePath);

    if (!settings->isWrittable()) {
        L_ERROR("File configuration is no writtable. It's impossible to write ini configurlation file in path = " + installFilePath);
        return false;
    }

    settings->writeSettings();

    if (QFile::exists(installFilePath)) {
        return true;
    } else {
        L_ERROR("An error occurred on the configuration file " + installFilePath + ". File is not created. ");
        return false;
    }
}

bool InstallManager::extractResources()
{
    Settings * settings = Settings::getInstance();

    QPair<bool, QString> extractUpdater = m_treeManager->extractResourceToPath(m_treeManager->getUpdaterResourcesPath(), m_treeManager->getUpdaterFilePath());
    L_INFO(extractUpdater.second);

    QPair<bool, QString> extractLoader = m_treeManager->extractResourceToPath(m_treeManager->getLoaderResourcesPath(), m_treeManager->getLoaderFilePath());
    L_INFO(extractLoader.second);

    QPair<bool, QString> extractAppIcon = m_treeManager->extractResourceToPath(":/images/shortcut.ico", settings->getInstallLocation() + "/shortcut.ico");
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
        // - startup shorcut (run start computer)
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
        // startup shortcut
        if (settings->getRunAtStart()) {
            success = success && windowsShortcutManager->createStartShorcut(settings->getShortcutOfflineName(), settings->getShortcutAllUser());
        }
        return success;

    #elif TARGET_OS_MAC
        // MAC OS SHORTCUT
        return true;
    #endif
}

bool InstallManager::launchLoader()
{
    QStringList args;
    QProcess process;
    QString loaderFile = m_treeManager->getLoaderFilePath();

    if (!QFile::exists(loaderFile)) {
        L_ERROR("An error occured when launch file " + loaderFile + ". The file doesn't exist.");
        return false;
    }
    L_INFO("Launch file " + loaderFile);
    bool success = process.startDetached(loaderFile, args);
    if (!success) {
        L_ERROR("Error when launching file " + loaderFile);
    } else {
        L_INFO("Success launching file " + loaderFile);
    }
    return success;
}

void InstallManager::closeInstallation(bool _launchApplication)
{
    if (_launchApplication) {
        launchLoader();
    }
    L_INFO("End treatment, close application");
    qApp->exit();
}

void InstallManager::eventCloseInstallation(bool _launchApplication)
{
    closeInstallation(_launchApplication);
}
