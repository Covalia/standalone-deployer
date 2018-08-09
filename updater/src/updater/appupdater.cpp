#include "updater/appupdater.h"
#include "network/downloadmanager.h"
#include "updater/config.h"
#include "io/config.h"
#include "xml/deploymentxml.h"
#include "updater/hash_key.h"
#include "utils/hashmac/hashmac512.h"
#include "io/fileutils.h"
#include "settings/settings.h"
#include "io/unzip/zipextractor.h"
#include "settings/commandlinesingleton.h"

#include <QDirIterator>
#include <QCoreApplication>

AppUpdater::AppUpdater(const QUrl &_appUrl, const QDir &_appInstallDir, QObject * _parent) : QObject(_parent),
    m_updater(0),
    m_appPath(Utils::getAppPath()),
    m_remoteUpdaterVersion(""),
    m_localUpdaterVersion(""),
    m_remoteJavaVersion(""),
    m_localJavaVersion(""),
    m_encoding(""),
    m_mainClass(""),
    m_runnerClass(""),
    m_memory("")
{
    m_appUrl = _appUrl;

    QNetworkProxy proxy;
    // TODO récupérer ici la configuration du proxy.
    // proxy.setType(QNetworkProxy::HttpProxy);
    // proxy.setHostName("10.33.26.250");
    // proxy.setPort(3129);
    // proxy.setUser("user");
    // proxy.setPassword("password");

    m_updater = new DownloadManager(m_appPath.getTempDir(), _appUrl.resolved(UpdaterConfig::DeployRootPath + "/"), proxy, this);

    connect(m_updater, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(updateProgress(qint64,qint64)));
    connect(m_updater, SIGNAL(downloadSpeedMessage(const QString&)),
            SLOT(updateDownloadSpeedMessage(const QString&)));
    connect(m_updater, SIGNAL(remainingTimeMessage(const QString&)),
            SLOT(updateRemainingTimeMessage(const QString&)));
    connect(m_updater, SIGNAL(downloadFileMessage(const QString&)),
            SLOT(updateDownloadFileMessage(const QString&)));
    connect(m_updater, SIGNAL(totalDownloadProgress(qint64,qint64)),
            SLOT(updateTotalDownloadProgress(qint64,qint64)));
}

AppUpdater::~AppUpdater()
{
    delete m_updater;
}

void AppUpdater::start()
{
    L_INFO("Updater started");
    emit serverUrlMessage(m_appUrl);

    m_appPath.makeAppDirectories();

    // TODO disable progress bar for cnlp files
    QMultiMap<Application, QUrl> map;
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::AppCnlpRemoteFilename + CommandLineSingleton::getInstance()->getApplicationHttpArguments()));
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::LoaderCnlpRemoteFilename));
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::UpdaterCnlpRemoteFilename));
    map.insert(Application::getCnlpApplication(), QUrl(UpdaterConfig::JavaCnlpRemoteFilename));

    connect(m_updater, SIGNAL(downloadsFinished()),
            SLOT(cnlpDownloadFinished()));

    m_updater->setUrlListToDownload(map);
}

void AppUpdater::updateProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit downloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::updateDownloadSpeedMessage(const QString &_speed)
{
    emit downloadSpeedMessage(_speed);
}

void AppUpdater::updateRemainingTimeMessage(const QString &_time)
{
    emit remainingTimeMessage(_time);
}

void AppUpdater::updateDownloadFileMessage(const QString &_file)
{
    emit downloadFileMessage(_file);
}

void AppUpdater::updateTotalDownloadProgress(qint64 _bytesReceived, qint64 _bytesTotal)
{
    emit totalDownloadProgress(_bytesReceived, _bytesTotal);
}

void AppUpdater::cnlpDownloadFinished()
{
    L_INFO("CNLP files downloads finished");

    disconnect(m_updater, SIGNAL(downloadsFinished()),
               this, SLOT(cnlpDownloadFinished()));

    const QString loaderCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::LoaderCnlpLocalFilename);
    const QString updaterCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::UpdaterCnlpLocalFilename);
    const QString javaCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::JavaCnlpLocalFilename);
    const QString applicationCnlpPath = m_appPath.getTempCnlpDir().absoluteFilePath(UpdaterConfig::AppCnlpLocalFilename);
    L_INFO("File to read: " + loaderCnlpPath);
    L_INFO("File to read: " + updaterCnlpPath);
    L_INFO("File to read: " + javaCnlpPath);
    L_INFO("File to read: " + applicationCnlpPath);

    DeploymentXML loaderXml(loaderCnlpPath);
    DeploymentXML updaterXml(updaterCnlpPath);
    DeploymentXML javaXml(javaCnlpPath);
    DeploymentXML applicationXml(applicationCnlpPath);

    if (loaderXml.read() && updaterXml.read() && javaXml.read() && applicationXml.read()) {
        // retrieving remote updater version.
        m_remoteUpdaterVersion = updaterXml.getApplication().getVersion();
        L_INFO("Remote updater version: " + m_remoteUpdaterVersion);

        // retrieving application encoding
        m_encoding = applicationXml.getEncoding();
        // retrieving application main class
        m_mainClass = applicationXml.getMainClass();
        // retrieving application runner class
        m_runnerClass = applicationXml.getRunnerClass();

        // retrieving cnlp arguments
        m_arguments.clear();
        m_arguments << applicationXml.getArguments();

        // retrieving memory needed
        m_memory = applicationXml.getMemory();

        Settings * settings = Settings::getInstance();
        m_localUpdaterVersion = settings->getUpdaterVersion();
        L_INFO("Local updater version: " + m_localUpdaterVersion);
        m_localJavaVersion = settings->getJavaVersion();
        L_INFO("Local java version: " + m_localJavaVersion);

        const Application loaderApplication = Application::getLoaderApplication();
        const Application updaterApplication = Application::getUpdaterApplication();
        const Application javaApplication = Application::getJavaApplication();
        const Application appApplication = Application::getAppApplication();

        const QList<Download> loaderDownloads = loaderXml.getDownloads();
        const QList<Download> updaterDownloads = updaterXml.getDownloads();
        const QList<Download> javaDownloads = javaXml.getDownloads();
        const QList<Download> appDownloads = applicationXml.getDownloads();

        // retrieving remote java version.
        m_remoteJavaVersion = "";
        for (int i = 0; i < javaDownloads.size(); ++i) {
            m_remoteJavaVersion = javaDownloads[i].getVersion();
            // only one java version for one operating system
            L_INFO("Remote java version: " + m_remoteJavaVersion);
            break;
        }

        m_cnlpParsedFiles.clear();
        {
            QListIterator<Download> iterator(loaderDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(loaderApplication, iterator.next());
            }
        }
        {
            QListIterator<Download> iterator(updaterDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(updaterApplication, iterator.next());
            }
        }
        {
            QListIterator<Download> iterator(javaDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(javaApplication, iterator.next());
            }
        }
        {
            QListIterator<Download> iterator(appDownloads);
            while (iterator.hasNext()) {
                m_cnlpParsedFiles.insert(appApplication, iterator.next());
            }
        }

        processCnlpDownloadFileList();

        connect(m_updater, SIGNAL(downloadsFinished()),
                SLOT(applicationDownloadFinished()));

        const QMultiMap<Application, QString> nonAlreadyDownloadedFiles = getFilesNonAlreadyInTempDir(m_filesToDownload, m_cnlpParsedFiles);

        m_updater->setUrlListToDownload(nonAlreadyDownloadedFiles);
    } else {
        L_ERROR("Unable to read the application cnlp files");
        // TODO handle error
    }
}

void AppUpdater::applicationDownloadFinished()
{
    disconnect(m_updater, SIGNAL(downloadsFinished()),
               this, SLOT(applicationDownloadFinished()));

    L_INFO("Files successfully downloaded");

    if (checkDownloadsAreOk()) {
        // all downloads are done

        bool loaderInstalledOk = true;
        bool updaterInstalledOk = true;
        bool javaInstalledOk = true;
        bool appInstalledOk = true;

        // we install only if the download list is not empty or if the remain list is not empty.
        if (doesAppNeedToBeRebuild(Application::getLoaderApplication())) {
            L_INFO("Need to rebuild and install " + Application::getLoaderApplication().getName());
            loaderInstalledOk &= buildApplicationInTempDirectory(Application::getLoaderApplication());
            L_INFO(Application::getLoaderApplication().getName() + " build result: " + QString::number(loaderInstalledOk));

            if (loaderInstalledOk) {
                loaderInstalledOk &= installLoader();

                if (loaderInstalledOk) {
                    L_INFO("No error reported on loader installation.");

                    // install loader cnlp file
                    if (installCnlpFile(UpdaterConfig::LoaderCnlpLocalFilename)) {
                        L_INFO("Need a restart for Loader.");
                        // restart process
                        QStringList args = CommandLineSingleton::getInstance()->getAllArguments();
                        if (m_appPath.startLoader(args)) {
                            if (CommandLineSingleton::getInstance()->isDebugMode()) {
                                // debug mode, print args
                                L_INFO("Restart loader with: " + args.join(" "));
                            } else {
                                L_INFO("Restart loader...");
                            }
                        } else {
                            L_ERROR("Loader restart failed.");
                        }
                    } else {
                        L_ERROR("Error installing loader cnlp.");
                    }

                    QCoreApplication::quit();
                    return;
                } else {
                    L_ERROR("Errors have been reported on loader installation.");
                }
            } else {
                L_ERROR("Errors have been reported on loader build.");
            }
        }

        if (doesAppNeedToBeRebuild(Application::getUpdaterApplication())) {
            L_INFO("Need to rebuild and install " + Application::getUpdaterApplication().getName());
            updaterInstalledOk &= buildApplicationInTempDirectory(Application::getUpdaterApplication());
            L_INFO(Application::getUpdaterApplication().getName() + " build result: " + QString::number(updaterInstalledOk));

            if (updaterInstalledOk) {
                updaterInstalledOk &= installUpdater();

                if (updaterInstalledOk) {
                    L_INFO("No error reported on updater installation.");

                    // install updater cnlp file
                    if (installCnlpFile(UpdaterConfig::UpdaterCnlpLocalFilename)) {
                        L_INFO("Need a restart for Updater.");
                        // restart process
                        QStringList args = CommandLineSingleton::getInstance()->getAllArguments();
                        if (m_appPath.startLoader(args)) {
                            if (CommandLineSingleton::getInstance()->isDebugMode()) {
                                // debug mode, print args
                                L_INFO("Restart loader with: " + args.join(" "));
                            } else {
                                L_INFO("Restart loader...");
                            }
                        } else {
                            L_ERROR("Loader restart failed.");
                        }
                    } else {
                        L_ERROR("Error installing updater cnlp.");
                    }

                    QCoreApplication::quit();
                    return;
                } else {
                    L_ERROR("Errors have been reported on updater installation.");
                }
            } else {
                L_ERROR("Errors have been reported on updater build.");
            }
        }

        if (doesAppNeedToBeRebuild(Application::getJavaApplication())) {
            L_INFO("Need to rebuild and install " + Application::getJavaApplication().getName());
            javaInstalledOk &= buildApplicationInTempDirectory(Application::getJavaApplication());
            L_INFO(Application::getJavaApplication().getName() + " build result: " + QString::number(javaInstalledOk));

            if (javaInstalledOk) {
                javaInstalledOk &= installJava();

                if (javaInstalledOk) {
                    L_INFO("No error reported on java installation.");

                    // install java cnlp file
                    if (!installCnlpFile(UpdaterConfig::JavaCnlpLocalFilename)) {
                        L_ERROR("Error installing java cnlp.");
                    }
                } else {
                    L_ERROR("Errors have been reported on java installation.");
                }
            } else {
                L_ERROR("Errors have been reported on java build.");
            }
        } else {
            L_INFO("Java does not need to be updated.");

            if (javaInstalledOk) {
                // dist directory was manually removed...
                const QString javaRemoteDistDir = m_appPath.getJavaDistDir(m_remoteJavaVersion).absolutePath();

                if (!FileUtils::directoryExists(javaRemoteDistDir)) {
                    L_INFO("Java dist directory does not exist. " + javaRemoteDistDir);
                    if (m_appPath.prepareJava(m_remoteJavaVersion, false)) {
                        L_INFO("Java " + m_remoteJavaVersion + " soft prepared.");
                    } else {
                        L_INFO("Unable to soft prepare Java " + m_remoteJavaVersion + ".");
                        javaInstalledOk = false;
                    }
                }
            }
        }

        if (doesAppNeedToBeRebuild(Application::getAppApplication())) {
            L_INFO("Need to rebuild and install " + Application::getAppApplication().getName());
            appInstalledOk &= buildApplicationInTempDirectory(Application::getAppApplication());
            L_INFO(Application::getAppApplication().getName() + " build result: " + QString::number(appInstalledOk));

            if (appInstalledOk) {
                appInstalledOk &= installApp();

                if (appInstalledOk) {
                    L_INFO("No error reported on application installation.");

                    // install app cnlp file
                    if (!installCnlpFile(UpdaterConfig::AppCnlpLocalFilename)) {
                        L_ERROR("Error installing application cnlp.");
                    }
                } else {
                    L_ERROR("Errors have been reported on application installation.");
                }
            } else {
                L_ERROR("Errors have been reported on application build.");
            }
        }

        if (loaderInstalledOk && updaterInstalledOk && javaInstalledOk && appInstalledOk) {
            // application, loader and updater didn't throw error.

            // rewrite cnlp files, in case they was removed manually
            installCnlpFile(UpdaterConfig::LoaderCnlpLocalFilename);
            installCnlpFile(UpdaterConfig::UpdaterCnlpLocalFilename);
            installCnlpFile(UpdaterConfig::JavaCnlpLocalFilename);
            installCnlpFile(UpdaterConfig::AppCnlpLocalFilename);

            bool native_extracted = true;

            const QString extractDir = m_appPath.getAppNativesDir().absolutePath();

            // optimisation: if extractDir already exists, no extraction is done
            if (!FileUtils::directoryExists(extractDir)) {
                L_INFO("Natives directory " + extractDir + " does not exist. Extracting...");

                // find the native jar and extract it
                QListIterator<Download> iterator(m_cnlpParsedFiles.values(Application::getAppApplication()));
                while (iterator.hasNext()) {
                    const Download & download = iterator.next();

                    if (download.isNative()) {
                        const QString fileToExtract = m_appPath.getAppDir().absoluteFilePath(download.getHref());

                        // extract zip to dist
                        ZipExtractor zip(fileToExtract, extractDir);

                        // start extraction
                        zip.extract();

                        if (zip.isOk()) {
                            L_INFO(fileToExtract + " extracted to " + extractDir);
                        } else {
                            L_ERROR(fileToExtract + " can not be extracted to " + extractDir);
                            native_extracted = false;
                            break;
                        }
                    }
                }
            } else {
                L_INFO("Natives directory " + extractDir + " already exists. No extraction done.");
            }

            if (native_extracted) {
                const QString classpathSeparator = m_appPath.getClasspathSeparator();

                QString classpath = "";

                // build the classpath string
                const QDir installDir = m_appPath.getInstallationDir();
                QListIterator<Download> iterator(m_cnlpParsedFiles.values(Application::getAppApplication()));
                while (iterator.hasNext()) {
                    const Download & download = iterator.next();

                    if (download.isMain() || (!download.isNative() && download.getOs() == DeploymentXML::getCurrentOsValue())) {
                        const QString fileToExtract = m_appPath.getAppDir().absoluteFilePath(download.getHref());
                        const QString relativeFile = installDir.relativeFilePath(fileToExtract);
                        L_INFO("Add " + relativeFile + " to classpath.");
                        if (classpath.isEmpty()) {
                            classpath += relativeFile;
                        } else {
                            classpath += classpathSeparator + relativeFile;
                        }
                    }
                }

                Settings * settings = Settings::getInstance();

                if (m_appPath.startPostInstallTasks(settings->getJavaVersion(), m_memory, classpath,
                                                    m_runnerClass, m_encoding, settings->getDataLocation())) {
                    L_INFO("Post install tasks started and finished.");

                    if (m_appPath.startApplication(settings->getJavaVersion(), m_memory, classpath, m_mainClass,
                                                   m_encoding, settings->getDataLocation(), m_arguments)) {
                        // clean temp directory
                        if (FileUtils::removeDirRecursively(m_appPath.getTempDir().absolutePath())) {
                            L_INFO("Cleaned " + m_appPath.getTempDir().absolutePath());
                        } else {
                            L_WARN("Can not clean " + m_appPath.getTempDir().absolutePath());
                        }

                        // clean old updater versions
                        L_INFO("Clean old updater versions");
                        if (m_appPath.cleanUpdaterDirExceptVersion(m_remoteUpdaterVersion)) {
                            L_INFO("Old updater versions cleaned");
                        } else {
                            L_WARN("Unable to clean old updater versions");
                        }

                        // clean old java versions
                        L_INFO("Clean old java versions");
                        if (m_appPath.cleanJavaDirExceptVersion(m_remoteJavaVersion)) {
                            L_INFO("Old java versions cleaned");
                        } else {
                            L_WARN("Unable to clean old java versions");
                        }

                        // quit application
                        L_INFO("Quit application.");
                    } else {
                        L_ERROR("Unable to start application. Exiting.");
                    }
                } else {
                    L_ERROR("Error while running post install tasks. Exiting.");
                }

                QCoreApplication::quit();
                return;
            }
        } else {
            L_ERROR("Unable to install applications. Aborting.");
            // TODO handle error
        }
    } else {
        L_ERROR("Error occured when downloading files. Aborting.");
        // TODO handle error
    }
}

bool AppUpdater::buildApplicationInTempDirectory(const Application &_application)
{
    bool buildOk = true;
    QDir tempAppDir;
    QDir appDir;
    QDir appBuildDir;

    if (_application == Application::getAppApplication() ||
        _application == Application::getLoaderApplication() ||
        _application == Application::getUpdaterApplication() ||
        _application == Application::getJavaApplication()) {
        if (_application == Application::getAppApplication()) {
            tempAppDir = m_appPath.getTempAppDir();
            appDir = QDir(m_appPath.getAppDir());
            appBuildDir = m_appPath.getTempAppBuildDir();
        } else if (_application == Application::getLoaderApplication()) {
            tempAppDir = m_appPath.getTempLoaderDir();
            appDir = QDir(m_appPath.getLoaderDir());
            appBuildDir = m_appPath.getTempLoaderBuildDir();
        } else if (_application == Application::getUpdaterApplication()) {
            tempAppDir = m_appPath.getTempUpdaterDir();
            appDir = m_appPath.getUpdaterVersionDir(m_remoteUpdaterVersion);
            appBuildDir = m_appPath.getTempUpdaterBuildDir();
        } else if (_application == Application::getJavaApplication()) {
            tempAppDir = m_appPath.getTempJavaDir();
            appDir = m_appPath.getJavaVersionDir(m_remoteJavaVersion);
            appBuildDir = m_appPath.getTempJavaBuildDir();
        }
    } else {
        // nothing to do, return true
        return true;
    }

    // remove temp/{application}-build directory
    L_INFO("Removing previous build directory: " + appBuildDir.absolutePath());
    FileUtils::removeDirRecursively(appBuildDir.absolutePath());

    if (QDir().mkpath(appBuildDir.absolutePath())) {
        L_INFO("Creating empty build directory: " + appBuildDir.absolutePath());

        const QList<QString> downloadedFiles = m_filesToDownload.values(_application);

        QListIterator<QString> downloadedIterator(downloadedFiles);
        while (downloadedIterator.hasNext()) {
            const QString downloadedFile = downloadedIterator.next();
            const QString localSourceFile = tempAppDir.absoluteFilePath(downloadedFile);
            const QString destinationFile = appBuildDir.absoluteFilePath(downloadedFile);

            if (QFile::exists(localSourceFile)) {
                L_INFO("download source: " + localSourceFile);
                L_INFO("destination:     " + destinationFile);
                QFileInfo destinationInfo(destinationFile);
                if (QDir().mkpath(destinationInfo.dir().absolutePath())) {
                    if (QFile(destinationFile).exists()) {
                        buildOk &= QFile(destinationFile).remove();
                    }
                    buildOk &= QDir().rename(localSourceFile, destinationFile);
                } else {
                    buildOk = false;
                }
            } else {
                buildOk = false;
            }
        }

        const QList<QString> keepFiles = m_filesToKeep.values(_application);

        QListIterator<QString> keepIterator(keepFiles);
        while (keepIterator.hasNext()) {
            const QString keepFile = keepIterator.next();
            const QString localSourceFile = appDir.absoluteFilePath(keepFile);
            const QString destinationFile = appBuildDir.absoluteFilePath(keepFile);

            if (QFile::exists(localSourceFile)) {
                L_INFO("keep source: " + localSourceFile);
                L_INFO("destination: " + destinationFile);
                QFileInfo destinationInfo(destinationFile);
                if (QDir().mkpath(destinationInfo.dir().absolutePath())) {
                    if (QFile(destinationFile).exists()) {
                        buildOk &= QFile(destinationFile).remove();
                    }
                    // we copy to not break current install
                    buildOk &= QFile(localSourceFile).copy(destinationFile);
                } else {
                    buildOk = false;
                }
            } else {
                buildOk = false;
            }
        }
    } else {
        buildOk = false;
    }

    return buildOk;
}

QList<QString> AppUpdater::getLocalFiles(const Application &_application)
{
    AppPath appPath = Utils::getAppPath();

    QList<QString> fileList;

    if (_application == Application::getAppApplication()
        || _application == Application::getLoaderApplication()
        || _application == Application::getUpdaterApplication()
        || _application == Application::getJavaApplication()) {
        QDir dir;

        if (_application == Application::getAppApplication()) {
            dir = QDir(appPath.getAppDir());
        } else if (_application == Application::getLoaderApplication()) {
            dir = QDir(appPath.getLoaderDir());
        } else if (_application == Application::getUpdaterApplication()) {
            dir = appPath.getUpdaterVersionDir(m_remoteUpdaterVersion);
        } else if (_application == Application::getJavaApplication()) {
            dir = appPath.getJavaVersionDir(m_remoteJavaVersion);
        }

        QDirIterator it(dir.absolutePath(), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filename = dir.relativeFilePath(it.next());

            // startWith tests use hardcoded "/" instead of QDir::separator() because
            // on windows, filename variable still contains "/", so tests will fail
            // if using QDir::separator()...

#ifdef Q_OS_MACOS
                // to disable false application rebuilding,
                // on macos, we ignore updater/<version>/updater.app/
                if (_application == Application::getUpdaterApplication()
                    && filename.startsWith(IOConfig::UpdaterName + IOConfig::MacOsAppExtension + "/")) {
                    L_INFO("Ignoring file in Updater: " + filename);
                    continue;
                }
                // and loader/loader.app/
                if (_application == Application::getLoaderApplication()
                    && filename.startsWith(IOConfig::LoaderName + IOConfig::MacOsAppExtension + "/")) {
                    L_INFO("Ignoring file in Loader: " + filename);
                    continue;
                }

                // and .DS_Store on macos
                if (filename.toLower().endsWith(".ds_store")) {
                    L_INFO("Ignoring file in " + _application.getName() + ": " + filename);
                    continue;
                }
#endif

#ifdef Q_OS_WIN
                // and Thumbs.db on windows
                if (filename.toLower().endsWith("thumbs.db")) {
                    L_INFO("Ignoring file in " + _application.getName() + ": " + filename);
                    continue;
                }
#endif

            // and java/<java-version>/dist/
            if (_application == Application::getJavaApplication()
                && filename.startsWith(AppPathImpl::JavaSubDirName + "/")) {
                L_INFO("Ignoring file in Java: " + filename);
                continue;
            }

            // TODO check la valeur de filename
            // and application/natives/
            if (_application == Application::getAppApplication()
                && filename.startsWith(AppPathImpl::NativesDirName + "/")) {
                L_INFO("Ignoring file in Application: " + filename);
                continue;
            }

            fileList.append(filename);
        }
    }

    return fileList;
}

QMultiMap<Application, QString> AppUpdater::getLocalFiles()
{
    QMultiMap<Application, QString> fileList;

    {
        const Application loaderApplication = Application::getLoaderApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getLoaderApplication()));
        while (iterator.hasNext()) {
            fileList.insert(loaderApplication, iterator.next());
        }
    }
    {
        const Application updaterApplication = Application::getUpdaterApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getUpdaterApplication()));
        while (iterator.hasNext()) {
            fileList.insert(updaterApplication, iterator.next());
        }
    }
    {
        const Application javaApplication = Application::getJavaApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getJavaApplication()));
        while (iterator.hasNext()) {
            fileList.insert(javaApplication, iterator.next());
        }
    }
    {
        const Application appApplication = Application::getAppApplication();
        QListIterator<QString> iterator(getLocalFiles(Application::getAppApplication()));
        while (iterator.hasNext()) {
            fileList.insert(appApplication, iterator.next());
        }
    }

    return fileList;
}

void AppUpdater::processCnlpDownloadFileList()
{
    // empty the lists
    m_filesToDownload.clear();
    m_filesToKeep.clear();
    m_remainingFiles.clear();

    // reading hash key only once
    const QString hash_key = HashKey::readHashKey();

    // list of all local files (installed and in temporary directory)
    QMultiMap<Application, QString> allLocalFiles = AppUpdater::getLocalFiles();

    // iterate over files read from cnlp files
    QSetIterator<Application> applicationIterator(QSet<Application>::fromList(m_cnlpParsedFiles.keys()));
    while (applicationIterator.hasNext()) {
        const Application & application = applicationIterator.next();

        L_INFO("Processing Application: " + application.getName());

        const QList<Download> & parsedDownloads = m_cnlpParsedFiles.values(application);

        // local files of the current application, installed only
        QList<QString> localFilesOfCurrentApplication = allLocalFiles.values(application);

        // check files to keep or download
        QListIterator<Download> parsedIterator(parsedDownloads);
        while (parsedIterator.hasNext()) {
            const Download & parsedDownload = parsedIterator.next();

            L_INFO("Remote file: " + parsedDownload.getHref() + " has hash: " + parsedDownload.getHashMac().shortHashMac());

            if (localFilesOfCurrentApplication.contains(parsedDownload.getHref())) {
                QString localFile = "";
                if (application == Application::getAppApplication()) {
                    localFile = m_appPath.getAppDir().absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getLoaderApplication()) {
                    localFile = m_appPath.getLoaderDir().absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getUpdaterApplication()) {
                    localFile = m_appPath.getUpdaterVersionDir(m_remoteUpdaterVersion).absoluteFilePath(parsedDownload.getHref());
                } else if (application == Application::getJavaApplication()) {
                    if (m_localJavaVersion.isEmpty()) {
                        // special case: if local version is empty, on fresh install, we search for a fake file.
                        // this prevents errors if remote version already exists locally.
                        // this should never happen unless someone manually touch config file.
                        localFile = m_appPath.getJavaVersionDir(m_remoteJavaVersion).absoluteFilePath(parsedDownload.getHref() + "-fake");
                    } else {
                        localFile = m_appPath.getJavaVersionDir(m_remoteJavaVersion).absoluteFilePath(parsedDownload.getHref());
                    }
                }

                // must be true but we test anyway
                if (QFile::exists(localFile)) {
                    HashMacString hashmac = HashMac512::hashFromFile(localFile, hash_key);
                    L_INFO("Local file: " + localFile + " has hashmac: " + hashmac.shortHashMac());

                    if (hashmac == parsedDownload.getHashMac()) {
                        L_INFO("Local and remote files are identical. Add to keep list.");
                        // add file to download list because file is out of date
                        m_filesToKeep.insert(application, parsedDownload.getHref());
                        // remove from the local list
                        localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                    } else {
                        L_INFO("Local and remote files differ. Add to download list.");
                        // add file to download list because file is out of date
                        m_filesToDownload.insert(application, parsedDownload.getHref());
                        // remove from the local list
                        localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                    }
                } else {
                    L_INFO("Local file does not exist. Add to download list.");
                    // add file to download list because it doesn't exist
                    m_filesToDownload.insert(application, parsedDownload.getHref());
                    // remove from the local list
                    localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
                }
            } else {
                L_INFO("Local file does not exist. Add to download list.");
                // add file to download list because it doesn't exist
                m_filesToDownload.insert(application, parsedDownload.getHref());
                // remove from the local list
                localFilesOfCurrentApplication.removeAll(parsedDownload.getHref());
            }
        }

        // remaining files, those who no longer need to be in the app
        QListIterator<QString> iterator(localFilesOfCurrentApplication);
        while (iterator.hasNext()) {
            m_remainingFiles.insert(application, iterator.next());
        }
    }
}

bool AppUpdater::doesAppNeedToBeRebuild(const Application &_application)
{
    return !m_filesToDownload.values(_application).isEmpty()
           || !m_remainingFiles.values(_application).isEmpty();
}

bool AppUpdater::checkDownloadsAreOk() const
{
    // reading hash key only once
    const QString hash_key = HashKey::readHashKey();

    bool downloadsOk = true;

    // check downloaded hash mac
    QMapIterator<Application, QString> iterator(m_filesToDownload);
    while (iterator.hasNext()) {
        iterator.next();
        const Application & application = iterator.key();
        const QString downloadedFile = iterator.value();

        if (application == Application::getAppApplication()
            || application == Application::getLoaderApplication()
            || application == Application::getUpdaterApplication()
            || application == Application::getJavaApplication()) {
            QDir dir;

            if (application == Application::getAppApplication()) {
                dir = QDir(m_appPath.getTempAppDir());
            } else if (application == Application::getLoaderApplication()) {
                dir = QDir(m_appPath.getTempLoaderDir());
            } else if (application == Application::getUpdaterApplication()) {
                dir = QDir(m_appPath.getTempUpdaterDir());
            } else if (application == Application::getJavaApplication()) {
                dir = QDir(m_appPath.getTempJavaDir());
            }

            // temporary location of downloaded file
            QString localFile = dir.absoluteFilePath(downloadedFile);

            if (QFile::exists(localFile)) {
                // if this local temporary file exists
                // checking its hashmac
                HashMacString hashmac = HashMac512::hashFromFile(localFile, hash_key);

                // we find the download corresponding to the downloadedFile to get its expected hashmac
                bool found = false;
                QListIterator<Download> parsedIterator(m_cnlpParsedFiles.values(application));
                while (parsedIterator.hasNext()) {
                    const Download & download = parsedIterator.next();
                    if (download.getHref() == downloadedFile) {
                        found = true;
                        if (hashmac != download.getHashMac()) {
                            L_WARN("Bad hashmac, expected: " + download.getHashMac().shortHashMac() + ", found: " + hashmac.shortHashMac() + " for file: " + localFile);
                            downloadsOk = false;
                        }
                        break;
                    }
                }
                if (!found) {
                    // we print a warning if downloaded file was not found in the parsed downloads list
                    // we just log it, in case this happens, but this mustn't
                    L_WARN("Downloaded file not found in the parsed downloads list: " + downloadedFile);
                }
            } else {
                // if this file does not exist, we get an error
                L_ERROR("Downloaded file does not exist " + localFile);
                downloadsOk = false;
            }
        }
    }

    return downloadsOk;
}

bool AppUpdater::installLoader()
{
    // called only if loader needs to be rebuild, so we now install it.
    bool loaderInstalledOk = true;

    L_INFO("Installing " + Application::getLoaderApplication().getName());

    const QString loaderInstallDir = m_appPath.getLoaderDir().absolutePath();
    const QString loaderOldDir = m_appPath.getLoaderOldDir().absolutePath();
    const QString loaderBuildDir = m_appPath.getTempLoaderBuildDir().absolutePath();

    // remove an old loader directory if it already exists
    if (FileUtils::directoryExists(loaderOldDir)) {
        L_INFO("Existing old loader directory needs to be removed: " + loaderOldDir);

        if (FileUtils::removeDirRecursively(loaderOldDir)) {
            L_INFO("Removed " + loaderOldDir);
        } else {
            L_ERROR("Unable to remove " + loaderOldDir);
            loaderInstalledOk = false;
        }
    }

    if (loaderInstalledOk) {
        // we continue only if the old existing directory has been deleted

        // firstly, we rename the old app directory.
        if (QDir().rename(loaderInstallDir, loaderOldDir)) {
            L_INFO("Renamed " + loaderInstallDir + " to " + loaderOldDir);
            if (QDir().rename(loaderBuildDir, loaderInstallDir)) {
                L_INFO("Renamed " + loaderBuildDir + " to " + loaderInstallDir);
                if (FileUtils::removeDirRecursively(loaderOldDir)) {
                    L_INFO("Removed " + loaderOldDir);
                } else {
                    L_WARN("Unable to remove " + loaderOldDir);
                    // not an error
                }
            } else {
                L_ERROR("Unable to rename " + loaderBuildDir + " to " + loaderInstallDir);
                loaderInstalledOk = false;
            }
        } else {
            L_ERROR("Unable to rename " + loaderInstallDir + " to " + loaderOldDir);
            loaderInstalledOk = false;
        }
        // extract app from dmg (macos).
        if (m_appPath.prepareLoader()) {
            L_INFO("Loader prepared.");
        } else {
            L_ERROR("Unable to prepare Loader.");
            loaderInstalledOk = false;
        }
    }

    return loaderInstalledOk;
}

bool AppUpdater::installUpdater()
{
    // called only if updated needs to be rebuild, so we now install it.
    bool updaterInstalledOk = true;

    // we can not overwrite ourselves
    if (m_localUpdaterVersion != m_remoteUpdaterVersion) {
        L_INFO("Installing " + Application::getUpdaterApplication().getName());

        const QString updaterInstallDir = m_appPath.getUpdaterVersionDir(m_remoteUpdaterVersion).absolutePath();
        const QString updaterBuildDir = m_appPath.getTempUpdaterBuildDir().absolutePath();

        if (FileUtils::directoryExists(updaterInstallDir)) {
            // it should not exist with the previous condition, but in case it exists
            // (it is not used because local and remote versions differ), we need to remove it
            if (FileUtils::removeDirRecursively(updaterInstallDir)) {
                L_INFO("Removed " + updaterInstallDir);
            } else {
                L_ERROR("Unable to remove " + updaterInstallDir);
                updaterInstalledOk = false;
            }
        }

        if (updaterInstalledOk) {
            // we continue only if there is no error

            // local and remote versions differ
            if (QDir().rename(updaterBuildDir, updaterInstallDir)) {
                L_INFO("Renamed " + updaterBuildDir + " to " + updaterInstallDir);
                // nothing to remove. deletion will be done at next launch.
                // extract app from dmg (macos).
                if (m_appPath.prepareUpdater(m_remoteUpdaterVersion)) {
                    L_INFO("Updater " + m_remoteUpdaterVersion + " prepared.");
                    // write new updater version number
                    Settings * settings = Settings::getInstance();
                    settings->setUpdaterVersion(m_remoteUpdaterVersion);
                    if (settings->writeSettings()) {
                        L_INFO("Written version " + m_remoteUpdaterVersion + " to settings.");
                    } else {
                        L_ERROR("Unable to write version " + m_remoteUpdaterVersion + " to settings.");
                        updaterInstalledOk = false;
                    }
                } else {
                    L_ERROR("Unable to prepare Updater " + m_remoteUpdaterVersion + ".");
                    updaterInstalledOk = false;
                }
            } else {
                L_ERROR("Unable to rename " + updaterBuildDir + " to " + updaterInstallDir);
                updaterInstalledOk = false;
            }
        }
    } else {
        L_WARN("Remote and local updaters differ but have same version number, no possible update.");
    }

    return updaterInstalledOk;
}

bool AppUpdater::installJava()
{
    // called only if java needs to be rebuild, so we now install it.
    bool javaInstalledOk = true;

    // we can overwrite java because it must not be used by us.
    L_INFO("Installing " + Application::getJavaApplication().getName());

    const QString javaInstallDir = m_appPath.getJavaVersionDir(m_remoteJavaVersion).absolutePath();
    const QString javaBuildDir = m_appPath.getTempJavaBuildDir().absolutePath();

    if (FileUtils::directoryExists(javaInstallDir)) {
        // remove existing java dir if it exists
        if (FileUtils::removeDirRecursively(javaInstallDir)) {
            L_INFO("Removed " + javaInstallDir);
        } else {
            L_ERROR("Unable to remove " + javaInstallDir);
            javaInstalledOk = false;
        }
    }

    if (javaInstalledOk) {
        // we continue only if there is no error

        // local and remote versions differ
        if (QDir().rename(javaBuildDir, javaInstallDir)) {
            L_INFO("Renamed " + javaBuildDir + " to " + javaInstallDir);
            // nothing to remove. deletion will be done at next launch.
            // extract zip.
            if (m_appPath.prepareJava(m_remoteJavaVersion, true)) {
                L_INFO("Java " + m_remoteJavaVersion + " force prepared.");
                // write new java version number
                Settings * settings = Settings::getInstance();
                settings->setJavaVersion(m_remoteJavaVersion);
                if (settings->writeSettings()) {
                    L_INFO("Written version " + m_remoteJavaVersion + " to settings.");
                } else {
                    L_ERROR("Unable to write version " + m_remoteJavaVersion + " to settings.");
                    javaInstalledOk = false;
                }
            } else {
                L_ERROR("Unable to force prepare Java " + m_remoteJavaVersion + ".");
                javaInstalledOk = false;
            }
        } else {
            L_ERROR("Unable to rename " + javaBuildDir + " to " + javaInstallDir);
            javaInstalledOk = false;
        }
    }

    return javaInstalledOk;
}

bool AppUpdater::installApp()
{
    // called only if app needs to be rebuild, so we now install it.
    bool appInstalledOk = true;

    L_INFO("Installing " + Application::getAppApplication().getName());

    const QString appInstallDir = m_appPath.getAppDir().absolutePath();
    const QString appOldDir = m_appPath.getAppOldDir().absolutePath();
    const QString appBuildDir = m_appPath.getTempAppBuildDir().absolutePath();

    // remove an old app directory if it already exists
    if (FileUtils::directoryExists(appOldDir)) {
        L_INFO("Existing old application directory needs to be removed: " + appOldDir);

        if (FileUtils::removeDirRecursively(appOldDir)) {
            L_INFO("Removed " + appOldDir);
        } else {
            L_ERROR("Unable to remove " + appOldDir);
            appInstalledOk = false;
        }
    }

    if (appInstalledOk) {
        // we continue only if the old existing directory has been deleted

        // firstly, we rename the old app directory.
        if (QDir().rename(appInstallDir, appOldDir)) {
            L_INFO("Renamed " + appInstallDir + " to " + appOldDir);
            if (QDir().rename(appBuildDir, appInstallDir)) {
                L_INFO("Renamed " + appBuildDir + " to " + appInstallDir);
                if (FileUtils::removeDirRecursively(appOldDir)) {
                    L_INFO("Removed " + appOldDir);
                } else {
                    L_WARN("Unable to remove " + appOldDir);
                    // not an error
                }
            } else {
                L_ERROR("Unable to rename " + appBuildDir + " to " + appInstallDir);
                appInstalledOk = false;
            }
        } else {
            L_ERROR("Unable to rename " + appInstallDir + " to " + appOldDir);
            appInstalledOk = false;
        }
    }

    return appInstalledOk;
}

bool AppUpdater::installCnlpFile(const QString &_file)
{
    const QString cnlpInstallFile = m_appPath.getCnlpDir().absoluteFilePath(_file);
    const QString cnlpBuildFile = m_appPath.getTempCnlpDir().absoluteFilePath(_file);

    bool cnlpInstalledOk = true;

    if (QFile::exists(cnlpInstallFile)) {
        L_INFO(cnlpInstallFile + " already exists. Removing.");
        if (QFile::remove(cnlpInstallFile)) {
            L_INFO(cnlpInstallFile + " removed.");
        } else {
            L_ERROR("Unable to remove " + cnlpInstallFile);
            cnlpInstalledOk = false;
        }
    }

    if (cnlpInstalledOk) {
        if (QFile::copy(cnlpBuildFile, cnlpInstallFile)) {
            L_INFO("Copy " + cnlpBuildFile + " to " + cnlpInstallFile);
        } else {
            L_ERROR("Unable to copy " + cnlpBuildFile + " to " + cnlpInstallFile);
            cnlpInstalledOk = false;
        }
    }

    return cnlpInstalledOk;
}

QMultiMap<Application, QString> AppUpdater::getFilesNonAlreadyInTempDir(const QMultiMap<Application, QString> _fullDownloadMap,
                                                                        const QMultiMap<Application, Download> _cnlpParsedFiles)
{
    // copy list
    QMultiMap<Application, QString> outMap = _fullDownloadMap;

    // reading hash key only once
    const QString hash_key = HashKey::readHashKey();

    // check downloaded hash mac
    QMutableMapIterator<Application, QString> iterator(outMap);
    while (iterator.hasNext()) {
        iterator.next();
        const Application & application = iterator.key();
        const QString downloadedFile = iterator.value();

        if (application == Application::getAppApplication()
            || application == Application::getLoaderApplication()
            || application == Application::getUpdaterApplication()
            || application == Application::getJavaApplication()) {
            QDir dir;

            const AppPath appPath = Utils::getAppPath();

            if (application == Application::getAppApplication()) {
                dir = QDir(appPath.getTempAppDir());
            } else if (application == Application::getLoaderApplication()) {
                dir = QDir(appPath.getTempLoaderDir());
            } else if (application == Application::getUpdaterApplication()) {
                dir = QDir(appPath.getTempUpdaterDir());
            } else if (application == Application::getJavaApplication()) {
                dir = QDir(appPath.getTempJavaDir());
            }

            // temporary location of downloaded file
            const QString localFile = dir.absoluteFilePath(downloadedFile);

            if (QFile::exists(localFile)) {
                // if this local temporary file exists
                // checking its hashmac
                HashMacString hashmac = HashMac512::hashFromFile(localFile, hash_key);

                // we find the download corresponding to the downloadedFile to get its expected hashmac
                QListIterator<Download> parsedIterator(_cnlpParsedFiles.values(application));
                while (parsedIterator.hasNext()) {
                    const Download & download = parsedIterator.next();
                    if (download.getHref() == downloadedFile) {
                        if (hashmac == download.getHashMac()) {
                            L_INFO("File for application: " + application.getName() + " already downloaded: " + download.getHref());
                            // remove this from temp list
                            iterator.remove();
                        }
                        break;
                    }
                }
            }
        }
    }

    return outMap;
}
