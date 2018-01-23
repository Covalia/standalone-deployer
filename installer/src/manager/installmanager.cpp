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
#include "shortcut/windowsshortcut.h"
#include "commandline/commandlineparser.h"
#include "settings/settings.h"
#include "settings/resourcessettings.h"
#include "lang/languagemanager.h"
#include "gui/style/stylemanager.h"
#include "fs/config.h"

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
    m_projectSettings = ResourcesSettings::getInstance();
    m_projectSettings->initSettings(":/resources/project.ini");
    m_projectSettings->readSettings();
    m_projectSettings->sendToSettings();
    L_INFO("Ip=" + m_projectSettings->getIp_server());

    L_INFO("Parsing command line");
    CommandLineParser * lineParser = new CommandLineParser();
    runAppAfter = lineParser->isRunApp();
    L_INFO("Run after = " + QString::number(runAppAfter));
    lineParser->sendToSettings();

    m_settings = Settings::getInstance();

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

    delete lineParser;
}

void InstallManager::eventStartInstallation()
{
    startInstallation();
}

void InstallManager::startInstallation()
{
    L_INFO("End of parameters initialization");

    L_INFO("Settings before start installation  : \n********\n" + m_settings->paramListString() + "********\n");

    QString errorMessage = "";
    m_treeManager = new AppTreeManager(QDir(m_settings->getInstallLocation()));

    // tree creation
    bool folderCreation = createInstallationFolders();
    if (!folderCreation) {
        errorMessage = tr("An error ocurred during folder creation");
    }

    moveLogInInstallFolder();

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
    bool succesCreateShortcut = createShortcut();
    if (!succesCreateShortcut) {
        errorMessage = tr("An error ocurred during shortcut creation");
    }

    bool success = folderCreation && settingWriting && succesExtractResources && succesCreateShortcut;

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
    L_INFO("Start the installation in directory : " + m_settings->getDataLocation());

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

void InstallManager::moveLogInInstallFolder()
{
    L_INFO("Move log in install folder");
    QString logPath(m_treeManager->getLogsDirPath().absolutePath() + "/installer.log");
    m_treeManager->extractResourceToPath("installer.log", logPath);
    new Logger(logPath);
    L_INFO("End move log in install folder");
}

bool InstallManager::createIniConfigurationFile()
{
    QString installFilePath(m_treeManager->getConfigurationFilePath());

    L_INFO("Start to create param file. Path = " + installFilePath);
    m_settings->initSettings(installFilePath);

    if (!m_settings->isWrittable()) {
        L_ERROR("File configuration is no writtable. It's impossible to write ini configurlation file in path = " + installFilePath);
        return false;
    }

    m_settings->writeSettings();

    if (QFile::exists(installFilePath)) {
        return true;
    } else {
        L_ERROR("An error occurred on the configuration file " + installFilePath + ". File is not created. ");
        return false;
    }
}

bool InstallManager::extractResources()
{
    QPair<bool, QString> extractUpdater = m_treeManager->extractResourceToPath(m_treeManager->getUpdaterResourcesPath(), m_treeManager->getUpdaterFilePath());
    L_INFO(extractUpdater.second);

    QPair<bool, QString> extractLoader = m_treeManager->extractResourceToPath(m_treeManager->getLoaderResourcesPath(), m_treeManager->getLoaderFilePath());
    L_INFO(extractLoader.second);

    QPair<bool, QString> extractAppIcon = m_treeManager->extractResourceToPath(":/images/shortcut.ico", m_treeManager->getImagesDirPath().absolutePath() + "/shortcut.ico");
    L_INFO(extractAppIcon.second);

    QPair<bool, QString> extractTrashIcon = m_treeManager->extractResourceToPath(":/images/trash.ico", m_treeManager->getImagesDirPath().absolutePath() + "/trash.ico");
    L_INFO(extractTrashIcon.second);

    QPair<bool, QString> extractConfigIcon = m_treeManager->extractResourceToPath(":/images/config.ico", m_treeManager->getImagesDirPath().absolutePath() + "/config.ico");
    L_INFO(extractConfigIcon.second);

    return extractUpdater.first && extractLoader.first && extractAppIcon.first && extractTrashIcon.first && extractConfigIcon.first;
}

bool InstallManager::createShortcut()
{
    #ifdef Q_OS_WIN
        // WINDOWS SHORCUT
        // - online desktop shortcut
        // - offline desktop shorcut
        // - startup shorcut (run start computer)
        // - startMenu

        WindowsShortcut * windowsShortcutManager = new WindowsShortcut();
        bool success = true;
        // online shortcut
        if (m_settings->getShortcutOnline()) {
            success = success && windowsShortcutManager->createDesktopShortcut(m_settings->getShortcutName(), "");
        }
        // offline shortcut
        if (m_settings->getShortcutOffline()) {
            success = success && windowsShortcutManager->createDesktopShortcut(m_settings->getShortcutOfflineName(), m_settings->getShortcutOfflineArgs());
        }
        // startup shortcut
        if (m_settings->getRunAtStart()) {
            success = success && windowsShortcutManager->createStartShorcut(m_settings->getShortcutName(), m_settings->getShortcutAllUser());
        }
        // StratMenu folder and shortcut
        success = success && windowsShortcutManager->createStartMenuShorcut(QDir(m_settings->getInstallLocation()).dirName(), m_settings->getShortcutAllUser());
        return success;

    #endif
    #ifdef Q_OS_MACOS
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
