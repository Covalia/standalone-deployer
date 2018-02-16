#include "installmanager.h"

#include <QApplication>
#include <QCoreApplication>
#include <QIcon>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QThread>
#include <QDirIterator>
#include <QStringList>

#include "log/logger.h"
#include "factories/shortcut/shortcut.h"
#include "commandline/commandlineparser.h"
#include "settings/settings.h"
#include "settings/resourcessettings.h"
#include "lang/languagemanager.h"
#include "gui/style/stylemanager.h"
#include "io/config.h"

InstallManager::InstallManager() : QThread(),
    m_uiManager(0),
    m_appPath(Utils::getAppPath())
{
}

InstallManager::~InstallManager()
{
    delete m_uiManager;
}

void InstallManager::initInstallation()
{
    // init project resources
    m_projectSettings = ResourcesSettings::getInstance();
    m_projectSettings->initSettings(":/project.ini");
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
                         this, SLOT(eventStartInstallation()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
        QObject::connect(this, SIGNAL(endInstallation(bool,QString)),
                         m_uiManager, SLOT(eventEndInstallation(bool,QString)));
    } else {
        start();
    }

    delete lineParser;
}


void InstallManager::eventStartInstallation()
{
    start();
}

void InstallManager::run()
{
    startInstallation();
}


void InstallManager::startInstallation()
{
    L_INFO("End of parameters initialization");

    L_INFO("Settings before start installation  : \n********\n" + m_settings->paramListString() + "********\n");

    QString errorMessage = "";
    m_appPath.setInstallationRootPath(QDir(m_settings->getInstallLocation()));

    // tree creation
    bool folderCreation = createInstallationFolders();
    if (!folderCreation) {
        errorMessage = tr("An error ocurred during folder creation");
    }

    moveLogInInstallFolder();

    // create updater version folder
    bool successUpdaterVersionCreation =  createUpdaterFolderVersion();
    if (!successUpdaterVersionCreation) {
        errorMessage = tr("An error ocurred during updater version folder creation");
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
    bool succesCreateShortcut = createShortcut();
    if (!succesCreateShortcut) {
        errorMessage = tr("An error ocurred during shortcut creation");
    }

    bool success = folderCreation && settingWriting && successUpdaterVersionCreation && succesExtractResources && succesCreateShortcut;

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

    if (!m_appPath.createDirectoryIfNotExist()) {
        L_ERROR("Error when create installation folder ");
        return false;
    } else {
        L_INFO("Succes of installation folder verification ");

        if (!m_appPath.makeAppDirectories()) {
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
    QString logPath(m_appPath.getLogsDirPath().absolutePath() + "/installer.log");
    m_appPath.extractResourceToPath("installer.log", logPath);
    new Logger(logPath);
    L_INFO("End move log in install folder");
}

bool InstallManager::createUpdaterFolderVersion()
{
    bool success = m_appPath.makeDirectoryIfNotExists(m_appPath.getUpdaterDirPath(), m_appPath.getUpdaterVersion());

    m_settings->setUpdaterVersion(m_appPath.getUpdaterVersion());
    return success;
}

bool InstallManager::createIniConfigurationFile()
{
    QString installFilePath(m_appPath.getConfigurationFilePath());

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
    QPair<bool, QString> extractUpdater = m_appPath.extractResourceToPath(m_appPath.getUpdaterResourcesPath(), m_appPath.getUpdaterFilePath(m_appPath.getUpdaterVersion()));
    if (extractUpdater.first) {
        L_INFO(extractUpdater.second);
    } else {
        L_ERROR(extractUpdater.second);
    }

    QPair<bool, QString> extractLoader = m_appPath.extractResourceToPath(m_appPath.getLoaderResourcesPath(), m_appPath.getLoaderFilePath());
    if (extractLoader.first) {
        L_INFO(extractLoader.second);
    } else {
        L_ERROR(extractLoader.second);
    }

    QPair<bool, QString> extractAppIcon = m_appPath.extractResourceToPath(":/images/shortcut.ico", m_appPath.getImagesDirPath().absolutePath() + "/shortcut.ico");
    if (extractAppIcon.first) {
        L_INFO(extractAppIcon.second);
    } else {
        L_ERROR(extractAppIcon.second);
    }

    QPair<bool, QString> extractTrashIcon = m_appPath.extractResourceToPath(":/images/trash.ico", m_appPath.getImagesDirPath().absolutePath() + "/trash.ico");
    if (extractTrashIcon.first) {
        L_INFO(extractTrashIcon.second);
    } else {
        L_ERROR(extractTrashIcon.second);
    }

    QPair<bool, QString> extractConfigIcon = m_appPath.extractResourceToPath(":/images/config.ico", m_appPath.getImagesDirPath().absolutePath() + "/config.ico");
    if (extractConfigIcon.first) {
        L_INFO(extractConfigIcon.second);
    } else {
        L_ERROR(extractConfigIcon.second);
    }

    QPair<bool, QString> extractStyle = m_appPath.extractResourceToPath(":/style.css", m_appPath.getConfigurationDirPath().absolutePath() + "/style.css");
    if (extractStyle.first) {
        L_INFO(extractStyle.second);
    } else {
        L_ERROR(extractStyle.second);
    }

    QDirIterator it(":/slideshow", QDirIterator::Subdirectories);
    bool extractSlides = true;
    while (it.hasNext()) {
        const QString resourcePath = it.next();
        QString resourceName = resourcePath.split("/").last();
        QPair<bool, QString> extractSlide = m_appPath.extractResourceToPath(resourcePath, m_appPath.getSlidesDirPath().absolutePath() + "/" + resourceName);
        if (extractSlide.first) {
            L_INFO(extractSlide.second);
        } else {
            L_ERROR(extractSlide.second);
        }
        extractSlides = extractSlides && extractSlide.first;
    }

    QPair<bool, QString> extractClose = m_appPath.extractResourceToPath(":/images/close.png", m_appPath.getImagesDirPath().absolutePath() + "/close.png");
    if (extractClose.first) {
        L_INFO(extractClose.second);
    } else {
        L_ERROR(extractClose.second);
    }

    QPair<bool, QString> extractTitleLogo = m_appPath.extractResourceToPath(":/images/logo_title.png", m_appPath.getImagesDirPath().absolutePath() + "/logo_title.png");
    if (extractTitleLogo.first) {
        L_INFO(extractTitleLogo.second);
    } else {
        L_ERROR(extractTitleLogo.second);
    }

    return extractUpdater.first && extractLoader.first && extractAppIcon.first && extractTrashIcon.first && extractConfigIcon.first && extractStyle.first && extractSlides && extractClose.first && extractTitleLogo.first;
}

bool InstallManager::createShortcut()
{
        Shortcut shortcut;
        bool success = true;
        // online shortcut
        if (m_settings->isShortcutOnline()) {
            success &= shortcut.createDesktopShortcut(m_appPath, m_settings->getShortcutName(), "", m_settings->getInstallLocation(), m_settings->getApplicationName());
        }
        // offline shortcut
        if (m_settings->isShortcutOffline()) {
            success &= shortcut.createDesktopShortcut(m_appPath, m_settings->getShortcutOfflineName(), m_settings->getShortcutOfflineArgs(), m_settings->getInstallLocation(), m_settings->getApplicationName());
        }
        // startup shortcut
        if (m_settings->isRunAtStart()) {
            success &= shortcut.createStartShorcut(m_appPath, m_settings->getShortcutName(), m_settings->isShortcutAllUser(), m_settings->getInstallLocation(), m_settings->getApplicationName());
        }
        // StartMenu folder and shortcut
        success &= shortcut.createStartMenuShorcut(m_appPath, QDir(m_settings->getInstallLocation()).dirName(), m_settings->isShortcutAllUser(), m_settings->getInstallLocation(), m_settings->getApplicationName());
        return success;
}

bool InstallManager::launchLoader()
{
    QStringList args;

    bool success = m_appPath.startLoader(args);
    if (!success) {
        L_ERROR("Error when launching loader");
    } else {
        L_INFO("Success launching loader");
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
