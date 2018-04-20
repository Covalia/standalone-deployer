#ifndef UPDATER__CONFIG_H
#define UPDATER__CONFIG_H

#include <QString>

class UpdaterConfig
{
public:
    // TODO lire depuis la config.ini
    /// URL de l'application
    static const QString AppUrl;

    // TODO inutile
    /// Chemin de l'installation
    static const QString InstallationDir;

    /// Updater User-Agent.
    static const QString UserAgentValue;

    /// Remote deploy root directory.
    static const QString DeployRootPath;

    /// Remote application cnlp filename.
    static const QString AppCnlpRemoteFilename;
    /// Local application cnlp filename.
    static const QString AppCnlpLocalFilename;

    /// Remote loader cnlp filename.
    static const QString LoaderCnlpRemoteFilename;
    /// Local loader cnlp filename.
    static const QString LoaderCnlpLocalFilename;

    /// Remote updater cnlp filename.
    static const QString UpdaterCnlpRemoteFilename;
    /// Local updater cnlp filename.
    static const QString UpdaterCnlpLocalFilename;

    /// Remote java cnlp filename.
    static const QString JavaCnlpRemoteFilename;
    /// Local java cnlp filename.
    static const QString JavaCnlpLocalFilename;

    /// Build directory suffix.
    static const QString BuildDirSuffix;
    /// Old directory suffix.
    static const QString OldDirSuffix;
};

#endif // ifndef UPDATER__CONFIG_H
