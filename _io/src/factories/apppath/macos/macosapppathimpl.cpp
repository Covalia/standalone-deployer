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

bool MacosAppPathImpl::extractAppFromDmgIfNotExist(const QString &_appName, const QFile &_dmgPath, const QFile &_appPath, const QString &_appInDmgPath, bool _forceOverwrite)
{
    bool result = true;

    L_INFO(QString("%1 dmg path: %2").arg(_appName).arg(_dmgPath.fileName()));
    L_INFO(QString("%1 app path: %2").arg(_appName).arg(_appPath.fileName()));
    L_INFO(QString("%1 app in dmg path: %2").arg(_appName).arg(_appInDmgPath));

    if (_forceOverwrite) {
        L_INFO(QString("%1 force overwrite").arg(_appName));
        if (FileUtils::removeDirRecursively(_appPath.fileName())) {
            L_INFO(QString("%1 app removed: %2").arg(_appName).arg(_appPath.fileName()));
        } else {
            L_ERROR(QString("%1 app cannot be removed: %2").arg(_appName).arg(_appPath.fileName()));
        }
    }

    if (!FileUtils::directoryExists(_appPath.fileName())) {
        L_INFO(QString("%1 app does not exist: %2").arg(_appName).arg(_appPath.fileName()));
        if (openDmgFile(_dmgPath)) {
            L_INFO(QString("Opening %1 dmg").arg(_appName));
            if (FileUtils::copyDirRecursively(_appInDmgPath, _appPath.fileName())) {
                L_INFO(QString("%1 app copied from dmg").arg(_appName));
            } else {
                L_ERROR(QString("Error while copying %1 app from dmg").arg(_appName));
                result = false;
            }
            if (closeDmgFile(_dmgPath)) {
                L_INFO(QString("Success closing %1 dmg").arg(_appName));
            } else {
                L_ERROR(QString("Error while closing %1 dmg").arg(_appName));
            }
        } else {
            L_ERROR(QString("Error while opening %1 dmg").arg(_appName));
            result = false;
        }
    } else {
        L_INFO(QString("%1 app already exists: %2").arg(_appName).arg(_appPath.fileName()));
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
    const QSharedPointer<QFile> exePath = QSharedPointer<QFile>(
                new QFile(appPath->fileName() + QDir::separator()
                          + "Contents" + QDir::separator() + "MacOS"
                          + QDir::separator() + IOConfig::LoaderFile));
    bool result = false;

    if (dmgPath && appPath && exePath) {
        const QString appInDmgPath = getMountDir().absoluteFilePath(IOConfig::LoaderFile + QDir::separator() + IOConfig::LoaderFile + IOConfig::MacOsAppExtension);
        result = extractAppFromDmgIfNotExist(app, *dmgPath, *appPath, appInDmgPath, false);
        result &= startComponent(exePath, _args);
    }

    return result;
}

bool MacosAppPathImpl::startUpdater(QString _version, QStringList _args)
{
    const QString app = IOConfig::UpdaterName;
    const QSharedPointer<QFile> dmgPath = getUpdaterFile(_version);
    const QSharedPointer<QFile> appPath = getUpdaterAppFile(_version);
    const QSharedPointer<QFile> exePath = QSharedPointer<QFile>(
                new QFile(appPath->fileName() + QDir::separator()
                          + "Contents" + QDir::separator() + "MacOS"
                          + QDir::separator() + IOConfig::UpdaterFile));
    bool result = false;

    if (dmgPath && appPath && exePath) {
        const QString appInDmgPath = getMountDir().absoluteFilePath(IOConfig::UpdaterFile + QDir::separator() + IOConfig::UpdaterFile + IOConfig::MacOsAppExtension);
        result = extractAppFromDmgIfNotExist(app, *dmgPath, *appPath, appInDmgPath, false);
        result &= startComponent(exePath, _args);
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

QString MacosAppPathImpl::getClasspathSeparator() const
{
    return ":";
}

bool MacosAppPathImpl::startComponent(QSharedPointer<QFile> _app, QStringList _args)
{
    if (!_app->exists()) {
        L_ERROR(QString("An error occured when launching %1. The exe file doesn't exist.").arg(_app->fileName()));
        return false;
    }

    L_INFO(QString("Launching file %1").arg(_app->fileName()));
    QProcess process;
    return process.startDetached(_app->fileName(), _args);
}
