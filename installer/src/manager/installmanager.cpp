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
#include "installerfactories/shortcut/shortcut.h"
#include "settings/settings.h"
#include "settings/resourcesettings.h"
#include "lang/languagemanager.h"
#include "io/config.h"

#include "installerfactories/osresources/osresources.h"

InstallManager::InstallManager() : QThread(),
    m_uiManager(nullptr),
    m_appPath(Utils::getAppPath()),
    m_projectSettings(nullptr),
    m_settings(nullptr),
    m_lineParser(),
    m_installLocation(""),
    m_dataLocation(""),
    m_proxyUsed(false),
    m_proxyHostname(""),
    m_proxyPort(""),
    m_proxyLogin(""),
    m_proxyPassword(""),
    m_locale(IOConfig::LocaleEnUs),
    m_runApp(false),
    m_runAtStart(false)
{
    L_INFO("Init InstallManager");
    // init project resources

    // extract project.ini to temp file
    QTemporaryFile projectIniFile;
    if (projectIniFile.open()) {
        if (OsResources::extractProjectIniToTempFile(projectIniFile.fileName())) {
            L_INFO("project.ini extracted from application resources.");
        } else {
            L_ERROR("Unable to open project.ini from application resources.");
            qApp->quit();
            return;
        }
        m_projectSettings = QSharedPointer<ResourceSettings>(new ResourceSettings(projectIniFile.fileName()));
    } else {
        L_ERROR("Unable to open temporary file for project.ini.");
        qApp->quit();
        return;
    }

    m_projectSettings->readSettings();

    L_INFO(QString("Deployment URL: %1").arg(m_projectSettings->getDeploymentUrl()));

    m_settings = Settings::getInstance();
    m_settings->setAppName(m_projectSettings->getAppName());
    m_settings->setDeploymentUrl(m_projectSettings->getDeploymentUrl());
    m_settings->setShortcutName(m_projectSettings->getShortcutName());
    m_settings->setStyle(m_projectSettings->getStyle());
}

InstallManager::~InstallManager()
{
    delete m_uiManager;
}

void InstallManager::initInstallation()
{
    const bool changeDataAllowed = m_projectSettings->isChangeDataLocationAllowed();

    if (m_lineParser.isLocaleSet()) {
        m_locale = m_lineParser.getLocale();
    } else {
        m_locale = LanguageManager::getSystemLocale();
    }

    if (m_lineParser.isSilent()) {
        if (!m_lineParser.getInstallLocation().isEmpty()) {
            m_installLocation = m_lineParser.getInstallLocation();
        } else {
            m_installLocation = m_projectSettings->getDefaultInstallationPath();
        }

        if (changeDataAllowed) {
            if (!m_lineParser.getDataLocation().isEmpty()) {
                m_dataLocation = m_lineParser.getDataLocation();
            } else {
                m_dataLocation = AppPath::getDataPathFromInstallPath(m_installLocation);
            }
        } else {
            m_dataLocation = AppPath::getDataPathFromInstallPath(m_installLocation);
        }

        m_proxyUsed = m_lineParser.isProxyUsed();
        m_proxyHostname = m_lineParser.getProxyHostname();
        m_proxyPort = m_lineParser.getProxyPort();
        m_proxyLogin = m_lineParser.getProxyLogin();
        m_proxyPassword = m_lineParser.getProxyPassword();

        m_runApp = m_lineParser.isRunApp();

        m_runAtStart = m_lineParser.isRunAtStart();

        start();
    } else {
        qApp->setApplicationName(QString(QObject::tr("Standalone deployment")));

        if (!m_uiManager) {
            m_uiManager = new UIManager(m_projectSettings->getAppName(), m_projectSettings->isChangeDataLocationAllowed());
        }

        m_uiManager->init(m_locale);

        // changing any setting via command line triggers a custom installation

        QString currentInstallPath;
        if (!m_lineParser.getInstallLocation().isEmpty()) {
            currentInstallPath = m_lineParser.getInstallLocation();
            m_uiManager->setInstallationFolder(m_lineParser.getInstallLocation());
            m_uiManager->setCustomInstallation(true);
        } else {
            currentInstallPath = m_projectSettings->getDefaultInstallationPath();
            m_uiManager->setInstallationFolder(m_projectSettings->getDefaultInstallationPath());
        }

        if (!m_lineParser.getDataLocation().isEmpty()) {
            m_uiManager->setDataFolder(m_lineParser.getDataLocation());
            m_uiManager->setCustomInstallation(true);
        } else {
            m_uiManager->setDataFolder(AppPath::getDataPathFromInstallPath(currentInstallPath));
        }

        if (m_lineParser.isProxyUsed()) {
            m_uiManager->setCustomInstallation(true);
        }
        m_uiManager->setProxyUsed(m_lineParser.isProxyUsed());

        if (m_lineParser.isProxyHostnameSet()) {
            m_uiManager->setProxyHostname(m_lineParser.getProxyHostname());
            m_uiManager->setCustomInstallation(true);
        }

        if (m_lineParser.isProxyPortSet()) {
            m_uiManager->setProxyPort(static_cast<quint16>(m_lineParser.getProxyPort().toUInt()));
            m_uiManager->setCustomInstallation(true);
        }

        if (m_lineParser.isProxyLoginSet()) {
            m_uiManager->setProxyLogin(m_lineParser.getProxyLogin());
            m_uiManager->setCustomInstallation(true);
        }

        if (m_lineParser.isProxyPasswordSet()) {
            m_uiManager->setProxyPassword(m_lineParser.getProxyPassword());
            m_uiManager->setCustomInstallation(true);
        }

        m_uiManager->setStartedAppWhenInstalled(m_lineParser.isRunApp());

        if (m_lineParser.isRunAtStart()) {
            m_uiManager->setCustomInstallation(true);
        }
        m_uiManager->setLaunchedAppAtStartUp(m_lineParser.isRunAtStart());

        connect(m_uiManager, SIGNAL(wizardFinishedSignal()),
                this, SLOT(start()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
        connect(this, SIGNAL(endInstallation(bool,QStringList)),
                m_uiManager, SLOT(eventEndInstallation(bool,QStringList)));
    }
}

void InstallManager::run()
{
    if (m_uiManager) {
        m_uiManager->printWizard();
    }

    const bool changeDataAllowed = m_projectSettings->isChangeDataLocationAllowed();

    if (!m_lineParser.isSilent()) {
        // update user choices

        if (m_uiManager->isCustomInstallation()) {
            // user has chosen custom installation

            m_installLocation = m_uiManager->getInstallationFolder();

            if (changeDataAllowed) {
                if (m_uiManager->isDataFolderChosen()) {
                    m_dataLocation = m_uiManager->getDataFolder();
                } else if (m_lineParser.getDataLocation().isEmpty()) {
                    m_dataLocation = AppPath::getDataPathFromInstallPath(m_installLocation);
                } else {
                    m_dataLocation = m_lineParser.getDataLocation();
                }
            } else {
                m_dataLocation = AppPath::getDataPathFromInstallPath(m_installLocation);
            }

            m_proxyUsed = m_uiManager->isProxyUsed();
            m_proxyHostname = m_uiManager->getProxyHostname();
            m_proxyPort = QString::number(m_uiManager->getProxyPort());
            m_proxyLogin = m_uiManager->getProxyLogin();
            m_proxyPassword = m_uiManager->getProxyPassword();

            m_runAtStart = m_uiManager->isLaunchedAppAtStartUp();
        } else {
            // user has chosen simple installation

            if (m_lineParser.getInstallLocation().isEmpty()) {
                m_installLocation = m_projectSettings->getDefaultInstallationPath();
            } else {
                m_installLocation = m_lineParser.getInstallLocation();
            }

            if (changeDataAllowed) {
                if (m_lineParser.getDataLocation().isEmpty()) {
                    m_dataLocation = AppPath::getDataPathFromInstallPath(m_installLocation);
                } else {
                    m_dataLocation = m_lineParser.getDataLocation();
                }
            } else {
                m_dataLocation = AppPath::getDataPathFromInstallPath(m_installLocation);
            }

            m_proxyUsed = false;
            m_proxyHostname = "";
            m_proxyPort = "1";
            m_proxyLogin = "";
            m_proxyPassword = "";

            m_runAtStart = false;
        }

        m_runApp = m_uiManager->isStartedAppWhenInstalled();
        m_locale = m_uiManager->getLocale();
    }

    L_INFO(QString("installLocation: %1").arg(m_installLocation));
    L_INFO(QString("dataLocation: %1").arg(m_dataLocation));
    L_INFO(QString("proxyUsed: %1").arg(QString(m_proxyUsed ? "yes" : "no")));
    L_INFO(QString("proxyHostname:%1").arg(m_proxyHostname));
    L_INFO(QString("proxyPort:%1").arg(m_proxyPort));
    L_INFO(QString("proxyLogin:%1").arg(m_proxyLogin));
    // no print of proxyPassword
    L_INFO(QString("locale: %1").arg(m_locale));
    L_INFO(QString("runApp: %1").arg(QString(m_runApp ? "yes" : "no")));
    L_INFO(QString("runAtStart: %1").arg(QString(m_runAtStart ? "yes" : "no")));

    m_appPath.setInstallationDir(QDir(m_installLocation));

    m_settings->setDataLocation(m_dataLocation);

    m_settings->setProxyUsed(m_proxyUsed);
    m_settings->setProxyHostname(m_proxyHostname);
    m_settings->setProxyPort(static_cast<int>(m_proxyPort.toUInt()));
    m_settings->setProxyLogin(m_proxyLogin);
    m_settings->setProxyPassword(m_proxyPassword);

    m_settings->setLocale(m_locale);

    m_settings->setRunAtStart(m_runAtStart);

    startInstallation();
}

void InstallManager::startInstallation()
{
    L_INFO(QString("Settings before installation start:\n********\n%1********\n").arg(m_settings->paramListString()));

    QStringList errorMessages;

    // tree creation
    bool successCreatingFolders = createInstallationFolders();
    if (!successCreatingFolders) {
        errorMessages << QT_TR_NOOP("Unable to create directory tree");
    }

    // create updater version folder
    bool successCreatingUpdaterVersion = createUpdaterFolderVersion();
    if (!successCreatingUpdaterVersion) {
        errorMessages << QT_TR_NOOP("Unable to create updater version folder");
    }

    // settings writing
    bool successWritingSettings = createIniConfigurationFile();
    if (!successWritingSettings) {
        errorMessages << QT_TR_NOOP("Unable to write settings file");
    }

    // extract resources
    bool successExtractingResources = extractResources();
    if (!successExtractingResources) {
        errorMessages << QT_TR_NOOP("Unable to extract resources");
    }

    // create shortcut
    bool successCreatingShortcut = createShortcut();
    if (!successCreatingShortcut) {
        errorMessages << QT_TR_NOOP("Unable to create shortcuts");
    }

    // preparing app
    bool successPreparingApp = m_appPath.prepareLoader() && m_appPath.prepareUpdater(m_appPath.getUpdaterVersion());
    if (!successPreparingApp) {
        errorMessages << QT_TR_NOOP("Unable to prepare applications");
    }

    bool success = successCreatingFolders && successWritingSettings
        && successCreatingUpdaterVersion && successExtractingResources
        && successCreatingShortcut && successPreparingApp;

    if (m_uiManager) {
        connect(m_uiManager, SIGNAL(quitInstaller(bool)), this, SLOT(closeInstallation(bool)));

        if (success) {
            L_INFO("Success Installation");
            emit endInstallation(success, errorMessages);
        } else {
            L_ERROR("Installation error");
            emit endInstallation(success, errorMessages);
        }
    } else {
        closeInstallation(m_runApp);
    }
}

bool InstallManager::cleanInstallationFolders()
{
    L_INFO("Cleaning sub installation folders.");

    bool result = true;

    result &= m_appPath.cleanAppDir();
    result &= m_appPath.cleanCnlpDir();
    result &= m_appPath.cleanImagesDir();
    result &= m_appPath.cleanJavaDir();
    result &= m_appPath.cleanLoaderDir();
    result &= m_appPath.cleanTempDir();
    result &= m_appPath.cleanUpdaterDir();

    return result;
}

bool InstallManager::createInstallationFolders()
{
    L_INFO(QString("Start the installation in directory: %1").arg(m_settings->getDataLocation()));

    if (!m_appPath.createDirectoryIfNotExist()) {
        L_ERROR("Error while creating installation folder.");
        return false;
    } else {
        L_INFO("Installation folder created.");

        // trying to clean old directories.
        if (!cleanInstallationFolders()) {
            // nothing to do
            L_WARN("Error while cleaning some directories. Continue.");
        }

        if (!m_appPath.makeAppDirectories()) {
            L_ERROR("Error when create sub-installation folder (application tree).");
            return false;
        }

        L_INFO("Succes of sub-installation folder creation ");

        return true;
    }
}

void InstallManager::moveLogIntoInstallFolder()
{
    QFile currentLogFile(Utils::getInstallerlLogPath());
    QFileInfo logFileInfo(currentLogFile);
    QFile destLogFile(m_appPath.getLogsDir().absoluteFilePath(logFileInfo.fileName()));

    m_appPath.extractResource(currentLogFile, destLogFile);
    currentLogFile.remove();
}

bool InstallManager::createUpdaterFolderVersion()
{
    bool success = m_appPath.makeDirectoryIfNotExisting(m_appPath.getUpdaterVersionDir(m_appPath.getUpdaterVersion()));

    m_settings->setUpdaterVersion(m_appPath.getUpdaterVersion());
    return success;
}

bool InstallManager::createIniConfigurationFile()
{
    QSharedPointer<QFile> installFilePath = m_appPath.getConfigurationFile();

    L_INFO(QString("Start to create param file. Path: %1").arg(installFilePath->fileName()));
    m_settings->initSettings(*installFilePath);

    if (!m_settings->isWritable()) {
        L_ERROR(QString("File configuration is no writable. It's impossible to write ini configuration file in path: %1").arg(installFilePath->fileName()));
        return false;
    }

    m_settings->writeSettings();

    if (installFilePath->exists()) {
        return true;
    } else {
        L_ERROR(QString("An error occurred on the configuration file %1. File is not created.").arg(installFilePath->fileName()));
        return false;
    }
}

bool InstallManager::extractResources()
{
    QPair<bool, QString> extractUpdater = m_appPath.extractResource(*m_appPath.getUpdaterResourcesFile(), *m_appPath.getUpdaterFile(m_appPath.getUpdaterVersion()));

    if (extractUpdater.first) {
        L_INFO(extractUpdater.second);
    } else {
        L_ERROR(extractUpdater.second);
    }

    QPair<bool, QString> extractLoader = m_appPath.extractResource(*m_appPath.getLoaderResourcesFile(), *m_appPath.getLoaderFile());
    if (extractLoader.first) {
        L_INFO(extractLoader.second);
    } else {
        L_ERROR(extractLoader.second);
    }

    OsResources osResources(m_appPath.getInstallationDir().absolutePath());
    bool extractResources = osResources.extractResources();

    return extractUpdater.first && extractLoader.first && extractResources;
}

bool InstallManager::createShortcut()
{
    Shortcut shortcut;
    bool success = true;

    // online shortcut
    success &= shortcut.createDesktopShortcut(m_appPath, m_settings->getShortcutName(), "", m_settings->getAppName());
    // startup shortcut
    if (m_settings->isRunAtStart()) {
        success &= shortcut.createStartShortcut(m_appPath, m_settings->getShortcutName(), m_settings->getAppName());
    }
    // StartMenu folder and shortcut
    success &= shortcut.createStartMenuShortcut(m_appPath, QDir(m_projectSettings->getDefaultInstallationPath()).dirName(), m_settings->getAppName());
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
        L_INFO("Application must be started.");
        launchLoader();
    }
    L_INFO("End treatment, close application");
    moveLogIntoInstallFolder();
    qApp->exit();
}
