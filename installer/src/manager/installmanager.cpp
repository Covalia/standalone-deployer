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
#include "settings/resourcesettings.h"
#include "lang/languagemanager.h"
#include "io/config.h"
#include "io/fileutils.h"

InstallManager::InstallManager() : QThread(),
    m_uiManager(0),
    m_appPath(Utils::getAppPath()),
    m_projectSettings(0),
    m_settings(0)
{
    // init project resources
    m_projectSettings = QSharedPointer<ResourceSettings>(new ResourceSettings(":/project.ini"));
    m_projectSettings->readSettings();
    m_projectSettings->writeAppSettings();

    setInstallationDir(m_projectSettings->getDefaultInstallationPath());
    L_INFO("Deployment URL: " + m_projectSettings->getDeploymentUrl());

    m_settings = Settings::getInstance();
}

InstallManager::~InstallManager()
{
    delete m_uiManager;
}

void InstallManager::initInstallation()
{
    L_INFO("Parsing command line");
    CommandLineParser lineParser;
    m_runAppAfter = lineParser.isRunApp();

    lineParser.sendToSettings();

    LanguageManager::initLanguage();

    // init values from project.ini and command line args.
    if (!lineParser.getInstallLocation().isEmpty()) {
        m_installationPath = lineParser.getInstallLocation();
    } else {
        m_installationPath = m_projectSettings->getDefaultInstallationPath();
    }

    if (!lineParser.getDataLocation().isEmpty()) {
        m_dataPath = lineParser.getDataLocation();
    } else {
        m_dataPath = m_projectSettings->getDefaultCustomInstallDataPath();
    }

    if (!lineParser.isSilent()) {
        qApp->setWindowIcon(QIcon(":/images/installer.png"));
        qApp->setApplicationName(QString(QObject::tr("Standalone deployment")));

        if (!m_uiManager) {
            m_uiManager = new UIManager(m_projectSettings->getAppName());
        }
        m_uiManager->init();

        m_uiManager->setInstallationFolder(m_installationPath);
        m_uiManager->setDataFolder(m_dataPath);

        m_uiManager->setCreatedOfflineShortcut(false);
        m_uiManager->setLaunchedAppAtStartUp(false);
        m_uiManager->setStartedAppWhenInstalled(true);

        connect(m_uiManager, SIGNAL(wizardFinishedSignal()),
                this, SLOT(start()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
        connect(this, SIGNAL(endInstallation(bool,QStringList)),
                m_uiManager, SLOT(eventEndInstallation(bool,QStringList)));
    } else {
        start();
    }
}

void InstallManager::run()
{
    startInstallation();
}

void InstallManager::startInstallation()
{
    L_INFO("End of parameters initialization");

    L_INFO("Settings before start installation  : \n********\n" + m_settings->paramListString() + "********\n");

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

    // create shorcut
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
            emit endInstallation(success, errorMessages);
        }
    } else {
        closeInstallation(m_runAppAfter);
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
    L_INFO("Start the installation in directory : " + m_settings->getDataLocation());

    if (!m_appPath.createDirectoryIfNotExist()) {
        L_ERROR("Error when create installation folder ");
        return false;
    } else {
        L_INFO("Succes of installation folder verification ");

        // trying to clean old directories.
        if (!cleanInstallationFolders()) {
            // nothing to do
            L_WARN("Error while cleaning some directories. Continue.");
        }

        if (!m_appPath.makeAppDirectories()) {
            L_ERROR("Error when create sub-installation folder (application tree) ");
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

    L_INFO("Start to create param file. Path = " + installFilePath->fileName());
    m_settings->initSettings(*installFilePath);

    if (!m_settings->isWritable()) {
        L_ERROR("File configuration is no writable. It's impossible to write ini configurlation file in path = " + installFilePath->fileName());
        return false;
    }

    m_settings->writeSettings();

    if (installFilePath->exists()) {
        return true;
    } else {
        L_ERROR("An error occurred on the configuration file " + installFilePath->fileName() + ". File is not created. ");
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

    QPair<bool, QString> extractAppIcon;
    {
        QFile fin(":/images/shortcut.ico");
        QFile fout(m_appPath.getImagesDir().absoluteFilePath("shortcut.ico"));
        extractAppIcon = m_appPath.extractResource(fin, fout);
        if (extractAppIcon.first) {
            L_INFO(extractAppIcon.second);
        } else {
            L_ERROR(extractAppIcon.second);
        }
    }

    QPair<bool, QString> extractTrashIcon;
    {
        QFile fin(":/images/trash.ico");
        QFile fout(m_appPath.getImagesDir().absoluteFilePath("trash.ico"));
        extractTrashIcon = m_appPath.extractResource(fin, fout);
        if (extractTrashIcon.first) {
            L_INFO(extractTrashIcon.second);
        } else {
            L_ERROR(extractTrashIcon.second);
        }
    }

    QPair<bool, QString> extractConfigIcon;
    {
        QFile fin(":/images/config.ico");
        QFile fout(m_appPath.getImagesDir().absoluteFilePath("config.ico"));
        extractConfigIcon = m_appPath.extractResource(fin, fout);
        if (extractConfigIcon.first) {
            L_INFO(extractConfigIcon.second);
        } else {
            L_ERROR(extractConfigIcon.second);
        }
    }

    QPair<bool, QString> extractStyle;
    {
        QFile fin(":/style.css");
        QFile fout(m_appPath.getConfigurationDir().absoluteFilePath("style.css"));
        extractStyle = m_appPath.extractResource(fin, fout);
        if (extractStyle.first) {
            L_INFO(extractStyle.second);
        } else {
            L_ERROR(extractStyle.second);
        }
    }

    QDirIterator it(":/slideshow", QDirIterator::Subdirectories);
    bool extractSlides = true;
    while (it.hasNext()) {
        QFile resourcePath(it.next());
        QFileInfo fileInfo(resourcePath);
        QFile fout(m_appPath.getSlidesDir().absoluteFilePath(fileInfo.fileName()));
        QPair<bool, QString> extractSlide = m_appPath.extractResource(resourcePath, fout);
        if (extractSlide.first) {
            L_INFO(extractSlide.second);
        } else {
            L_ERROR(extractSlide.second);
        }
        extractSlides &= extractSlide.first;
    }

    QPair<bool, QString> extractClose;
    {
        QFile fin(":/images/close.png");
        QFile fout(m_appPath.getImagesDir().absoluteFilePath("close.png"));
        extractClose = m_appPath.extractResource(fin, fout);
        if (extractClose.first) {
            L_INFO(extractClose.second);
        } else {
            L_ERROR(extractClose.second);
        }
    }

    QPair<bool, QString> extractTitleLogo;
    {
        QFile fin(":/images/logo_title.png");
        QFile fout(m_appPath.getImagesDir().absoluteFilePath("logo_title.png"));
        extractTitleLogo = m_appPath.extractResource(fin, fout);
        if (extractTitleLogo.first) {
            L_INFO(extractTitleLogo.second);
        } else {
            L_ERROR(extractTitleLogo.second);
        }
    }

    return extractUpdater.first && extractLoader.first && extractAppIcon.first && extractTrashIcon.first && extractConfigIcon.first && extractStyle.first && extractSlides && extractClose.first && extractTitleLogo.first;
}

bool InstallManager::createShortcut()
{
    Shortcut shortcut;
    bool success = true;

    // online shortcut
    if (m_settings->isShortcutOnline()) {
        success &= shortcut.createDesktopShortcut(m_appPath, m_settings->getShortcutName(), "", m_settings->getAppName());
    }
    // offline shortcut
    if (m_settings->isShortcutOffline()) {
        success &= shortcut.createDesktopShortcut(m_appPath, m_settings->getShortcutOfflineName(), m_settings->getShortcutOfflineArgs(), m_settings->getAppName());
    }
    // startup shortcut
    if (m_settings->isRunAtStart()) {
        success &= shortcut.createStartShorcut(m_appPath, m_settings->getShortcutName(), m_settings->isShortcutForAllUsers(), m_settings->getAppName());
    }
    // StartMenu folder and shortcut
    success &= shortcut.createStartMenuShorcut(m_appPath, QDir(m_projectSettings->getDefaultInstallationPath()).dirName(), m_settings->isShortcutForAllUsers(), m_settings->getAppName());
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
    moveLogIntoInstallFolder();
    qApp->exit();
}

void InstallManager::setInstallationDir(const QString &_directory)
{
    m_appPath.setInstallationDir(QDir(_directory));
}
