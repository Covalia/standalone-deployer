#include "factories/apppath/macos/macosapppathimpl.h"
#include "io/fileutils.h"

MacosAppPathImpl::MacosAppPathImpl(FileSystemConfig::AppComponent _app) : AppPathImpl(_app)
{
    QDir dir(QCoreApplication::applicationDirPath());

    switch (_app) {
        case FileSystemConfig::AppComponent::Loader:
            cdUp(dir, 4);
            break;
        case FileSystemConfig::AppComponent::Updater:
            cdUp(dir, 5);
            break;
        case FileSystemConfig::AppComponent::Uninstaller:
            cdUp(dir, 3);
            break;
        case FileSystemConfig::AppComponent::Installer:
            // nothing to do, call setInstallationRootPath manually only from installer project
            break;
    }
    m_installationDir = dir.absolutePath();
}

bool MacosAppPathImpl::makeAppDirectories()
{
    bool result = true;

    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::AppDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ConfigurationDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ExtensionDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::ImagesDir);
    result &= makeDirectoryIfNotExists(getImagesDirPath(), FileSystemConfig::SlidesDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::JavaDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LogsDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::TempDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::MountDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::UpdaterDir);
    result &= makeDirectoryIfNotExists(m_installationDir, FileSystemConfig::LoaderDir);

    return result;
}

QString MacosAppPathImpl::getLoaderResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::LoaderFile + FileSystemConfig::MacOsDmgExtension;
}

QString MacosAppPathImpl::getUpdaterResourcesPath()
{
    return ResourceBinPrefix + FileSystemConfig::UpdaterFile + FileSystemConfig::MacOsDmgExtension;
}

QString MacosAppPathImpl::getLoaderFilePath()
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::LoaderDir + "/" + FileSystemConfig::LoaderFile + FileSystemConfig::MacOsDmgExtension;
}

QString MacosAppPathImpl::getUpdaterFilePath(QString _updaterVersion)
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + _updaterVersion + "/" + FileSystemConfig::UpdaterFile + FileSystemConfig::MacOsDmgExtension;
}

QString MacosAppPathImpl::getLoaderAppFilePath()
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::LoaderDir + "/" + FileSystemConfig::LoaderFile + FileSystemConfig::MacOsAppExtension;
}

QString MacosAppPathImpl::getUpdaterAppFilePath(QString _updaterVersion)
{
    return m_installationDir.absolutePath() + "/" +  FileSystemConfig::UpdaterDir + "/" + _updaterVersion + "/" + FileSystemConfig::UpdaterFile + FileSystemConfig::MacOsAppExtension;
}

bool MacosAppPathImpl::startApplication(QString _app, QStringList _args)
{
    if (!QFile::exists(_app)) {
        L_ERROR("An error occured when launching " + _app + ". The app dir doesn't exist.");
        return false;
    }

    QStringList args;
    args << "-a";
    args << _app;
    if (!_args.isEmpty()) {
        args << "--args";
        args << _args;
    }

    L_INFO("Launching app " + _app);
    QProcess process;
    process.start("open", args);
    // open retourne lorsque le programme est lancé, donc on peut faire un waitForFinished :)
    return process.waitForFinished();
}

bool MacosAppPathImpl::extractAppFromDmgIfNotExist(const QString &_appName, const QString &_dmgPath, const QString &_appPath, const QString &_appInDmgPath)
{
    bool result = true;
    L_INFO(_appName + " dmg path: " + _dmgPath);
    L_INFO(_appName + " app path: " + _appPath);
    L_INFO(_appName + " app in dmg path: " + _appInDmgPath);

    if (!FileUtils::directoryExists(_appPath)) {
        L_INFO(_appName + " app does not exist: " + _appPath);
        if (openDmgFile(_dmgPath)) {
            L_INFO("Opening " + _appName + " dmg");
            if (FileUtils::copyDirRecursively(_appInDmgPath, _appPath)) {
                L_INFO(_appName + " app copied from dmg");
            } else {
                L_ERROR("Error while copying " + _appName + " app from dmg");
                result = false;
            }
            if (closeDmgFile(_dmgPath)) {
                L_INFO("Success closing " + _appName + " dmg");
            } else {
                L_ERROR("Error while closing " + _appName + " dmg");
            }
        } else {
            L_ERROR("Error while opening " + _appName + " dmg");
            result = false;
        }
    } else {
        L_INFO(_appName + " app already exists: " + _appPath);
    }
    return result;
}

bool MacosAppPathImpl::startLoader(QStringList _args)
{
    const QString app = "loader";
    const QString dmgPath = getLoaderFilePath();
    const QString appPath = getLoaderAppFilePath();
    const QString appInDmgPath = getMountDirPath().absoluteFilePath(FileSystemConfig::LoaderFile + QDir::separator() + FileSystemConfig::LoaderFile + FileSystemConfig::MacOsAppExtension);

    bool result = extractAppFromDmgIfNotExist(app, dmgPath, appPath, appInDmgPath);
    result &= startApplication(appPath, _args);
    return result;
}

bool MacosAppPathImpl::startUpdater(QString _version, QStringList _args)
{
    const QString app = "updater";
    const QString dmgPath = getUpdaterFilePath(_version);
    const QString appPath = getUpdaterAppFilePath(_version);
    const QString appInDmgPath = getMountDirPath().absoluteFilePath(FileSystemConfig::UpdaterFile + QDir::separator() + FileSystemConfig::UpdaterFile + FileSystemConfig::MacOsAppExtension);

    bool result = extractAppFromDmgIfNotExist(app, dmgPath, appPath, appInDmgPath);
    result &= startApplication(appPath, _args);
    return result;
}

bool MacosAppPathImpl::openDmgFile(const QString &_dmgFile)
{
    QStringList args;

    args << "attach";
    args << "-noautoopen";
    args << "-mountroot";
    args << getMountDirPath().absolutePath();
    args << _dmgFile;

    QProcess process;
    process.start("hdiutil", args);
    return process.waitForFinished();
}

bool MacosAppPathImpl::closeDmgFile(const QString &_dmgFile)
{
    QFileInfo file(_dmgFile);
    QString imageName = file.baseName();

    QStringList args;

    args << "detach";
    args << getMountDirPath().absoluteFilePath(imageName);

    QProcess process;
    process.start("hdiutil", args);
    return process.waitForFinished();
    return false;
}

QDir MacosAppPathImpl::getMountDirPath()
{
    return QDir(m_installationDir.filePath(FileSystemConfig::MountDir));
}
