#include "factories/apppath/macos/macosapppathimpl.h"
#include "io/fileutils.h"

MacosAppPathImpl::MacosAppPathImpl(IOConfig::AppComponent _app) : AppPathImpl(_app)
{
    QDir dir(QCoreApplication::applicationDirPath());

    switch (_app) {
        case IOConfig::AppComponent::Loader:
            cdUp(dir, 4);
            break;
        case IOConfig::AppComponent::Updater:
            cdUp(dir, 5);
            break;
        case IOConfig::AppComponent::Installer:
            // nothing to do, call setInstallationDir manually only from installer project
            break;
    }
    m_installationDir = dir.absolutePath();
}

bool MacosAppPathImpl::makeAppDirectories()
{
    bool result = true;

    result &= makeDirectoryIfNotExisting(getAppDir());
    result &= makeDirectoryIfNotExisting(getConfigurationDir());
    result &= makeDirectoryIfNotExisting(getExtensionDir());
    result &= makeDirectoryIfNotExisting(getImagesDir());
    result &= makeDirectoryIfNotExisting(QDir(getImagesDir().absoluteFilePath(IOConfig::SlidesDir))); // TODO
    result &= makeDirectoryIfNotExisting(getJavaDir());
    result &= makeDirectoryIfNotExisting(getLogsDir());
    result &= makeDirectoryIfNotExisting(getTempDir());
    result &= makeDirectoryIfNotExisting(getCnlpDir());
    result &= makeDirectoryIfNotExisting(getDataDir());
    result &= makeDirectoryIfNotExisting(getMountDir());
    result &= makeDirectoryIfNotExisting(getUpdaterDir());
    result &= makeDirectoryIfNotExisting(getLoaderDir());

    return result;
}

QSharedPointer<QFile> MacosAppPathImpl::getLoaderResourcesFile()
{
    return QSharedPointer<QFile>(new QFile(ResourceBinPrefix + IOConfig::LoaderFile + IOConfig::MacOsDmgExtension));
}

QSharedPointer<QFile> MacosAppPathImpl::getUpdaterResourcesFile()
{
    return QSharedPointer<QFile>(new QFile(ResourceBinPrefix + IOConfig::UpdaterFile + IOConfig::MacOsDmgExtension));
}

QSharedPointer<QFile> MacosAppPathImpl::getLoaderFile()
{
    return QSharedPointer<QFile>(new QFile(m_installationDir.absoluteFilePath(IOConfig::LoaderDir + QDir::separator() + IOConfig::LoaderFile + IOConfig::MacOsDmgExtension)));
}

QSharedPointer<QFile> MacosAppPathImpl::getUpdaterFile(QString _updaterVersion)
{
    return QSharedPointer<QFile>(new QFile(m_installationDir.absoluteFilePath(IOConfig::UpdaterDir + QDir::separator() + _updaterVersion + QDir::separator() + IOConfig::UpdaterFile + IOConfig::MacOsDmgExtension)));
}

QSharedPointer<QFile> MacosAppPathImpl::getLoaderAppFile()
{
    return QSharedPointer<QFile>(new QFile(m_installationDir.absoluteFilePath(IOConfig::LoaderDir + QDir::separator() + IOConfig::LoaderFile + IOConfig::MacOsAppExtension)));
}

QSharedPointer<QFile> MacosAppPathImpl::getUpdaterAppFile(QString _updaterVersion)
{
    return QSharedPointer<QFile>(new QFile(m_installationDir.absoluteFilePath(IOConfig::UpdaterDir + QDir::separator() + _updaterVersion + QDir::separator() + IOConfig::UpdaterFile + IOConfig::MacOsAppExtension)));
}

bool MacosAppPathImpl::startComponent(QSharedPointer<QFile> _app, QStringList _args)
{
    if (!_app->exists()) {
        L_ERROR("An error occured when launching " + _app->fileName() + ". The app dir doesn't exist.");
        return false;
    }

    QStringList args;
    args << "-a";
    args << _app->fileName();
    if (!_args.isEmpty()) {
        args << "--args";
        args << _args;
    }

    L_INFO("Launching app " + _app->fileName());
    QProcess process;
    process.start("open", args);
    // open retourne lorsque le programme est lancé, donc on peut faire un waitForFinished :)
    return process.waitForFinished();
}

bool MacosAppPathImpl::extractAppFromDmgIfNotExist(const QString &_appName, const QFile &_dmgPath, const QFile &_appPath, const QString &_appInDmgPath, bool _forceOverwrite)
{
    bool result = true;

    L_INFO(_appName + " dmg path: " + _dmgPath.fileName());
    L_INFO(_appName + " app path: " + _appPath.fileName());
    L_INFO(_appName + " app in dmg path: " + _appInDmgPath);

    if (_forceOverwrite) {
        L_INFO(_appName + " force overwrite");
        if (FileUtils::removeDirRecursively(_appPath.fileName())) {
            L_INFO(_appName + " app removed: " + _appPath.fileName());
        } else {
            L_ERROR(_appName + " app cannot be removed: " + _appPath.fileName());
        }
    }

    if (!FileUtils::directoryExists(_appPath.fileName())) {
        L_INFO(_appName + " app does not exist: " + _appPath.fileName());
        if (openDmgFile(_dmgPath)) {
            L_INFO("Opening " + _appName + " dmg");
            if (FileUtils::copyDirRecursively(_appInDmgPath, _appPath.fileName())) {
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
        L_INFO(_appName + " app already exists: " + _appPath.fileName());
    }
    return result;
}

bool MacosAppPathImpl::prepareLoader()
{
    const QString app = IOConfig::LoaderName;
    const QSharedPointer<QFile> dmgPath = getLoaderFile();
    const QSharedPointer<QFile> appPath = getLoaderAppFile();
    bool result = false;

    if (dmgPath && appPath) {
        const QString appInDmgPath = getMountDir().absoluteFilePath(IOConfig::LoaderFile + QDir::separator() + IOConfig::LoaderFile + IOConfig::MacOsAppExtension);
        result = extractAppFromDmgIfNotExist(app, *dmgPath, *appPath, appInDmgPath, true);
    }

    return result;
}

bool MacosAppPathImpl::prepareUpdater(QString _version)
{
    const QString app = IOConfig::UpdaterName;
    const QSharedPointer<QFile> dmgPath = getUpdaterFile(_version);
    const QSharedPointer<QFile> appPath = getUpdaterAppFile(_version);
    bool result = false;

    if (dmgPath && appPath) {
        const QString appInDmgPath = getMountDir().absoluteFilePath(IOConfig::UpdaterFile + QDir::separator() + IOConfig::UpdaterFile + IOConfig::MacOsAppExtension);
        result = extractAppFromDmgIfNotExist(app, *dmgPath, *appPath, appInDmgPath, true);
    }

    return result;
}

bool MacosAppPathImpl::startLoader(QStringList _args)
{
    const QString app = IOConfig::LoaderName;
    const QSharedPointer<QFile> dmgPath = getLoaderFile();
    const QSharedPointer<QFile> appPath = getLoaderAppFile();
    bool result = false;

    if (dmgPath && appPath) {
        const QString appInDmgPath = getMountDir().absoluteFilePath(IOConfig::LoaderFile + QDir::separator() + IOConfig::LoaderFile + IOConfig::MacOsAppExtension);
        result = extractAppFromDmgIfNotExist(app, *dmgPath, *appPath, appInDmgPath, false);
        result &= startComponent(appPath, _args);
    }

    return result;
}

bool MacosAppPathImpl::startUpdater(QString _version, QStringList _args)
{
    const QString app = IOConfig::UpdaterName;
    const QSharedPointer<QFile> dmgPath = getUpdaterFile(_version);
    const QSharedPointer<QFile> appPath = getUpdaterAppFile(_version);
    bool result = false;

    if (dmgPath && appPath) {
        const QString appInDmgPath = getMountDir().absoluteFilePath(IOConfig::UpdaterFile + QDir::separator() + IOConfig::UpdaterFile + IOConfig::MacOsAppExtension);
        result = extractAppFromDmgIfNotExist(app, *dmgPath, *appPath, appInDmgPath, false);
        result &= startComponent(appPath, _args);
    }

    return result;
}

bool MacosAppPathImpl::openDmgFile(const QFile &_dmgFile)
{
    QStringList args;

    args << "attach";
    args << "-noautoopen";
    args << "-mountroot";
    args << getMountDir().absolutePath();
    args << _dmgFile.fileName();

    QProcess process;
    process.start("hdiutil", args);
    return process.waitForFinished();
}

bool MacosAppPathImpl::closeDmgFile(const QFile &_dmgFile)
{
    QFileInfo file(_dmgFile);
    QString imageName = file.baseName();

    QStringList args;

    args << "detach";
    args << getMountDir().absoluteFilePath(imageName);

    QProcess process;
    process.start("hdiutil", args);
    return process.waitForFinished();
}

QDir MacosAppPathImpl::getMountDir()
{
    return QDir(m_installationDir.absoluteFilePath(IOConfig::MountDir));
}

QString MacosAppPathImpl::getJavaExecutablePath(const QString &_javaVersion) const
{
    return getJavaDistDir(_javaVersion).absoluteFilePath(QString("bin") + QDir::separator() + QString("java"));
}
