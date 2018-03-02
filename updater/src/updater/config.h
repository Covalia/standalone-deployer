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

    /// User-Agent de l'updater
    static const QString UserAgentValue;

    /// Remote deploy root directory
    static const QString DeployRootPath;

    /// Nom fichier application distant.
    static const QString AppCnlpRemoteFilename;
    /// Nom fichier application local.
    static const QString AppCnlpLocalFilename;

    /// Nom fichier loader distant.
    static const QString LoaderCnlpRemoteFilename;
    /// Nom fichier loader local.
    static const QString LoaderCnlpLocalFilename;

    /// Nom fichier updater distant.
    static const QString UpdaterCnlpRemoteFilename;
    /// Nom fichier updater local.
    static const QString UpdaterCnlpLocalFilename;
};

#endif // ifndef UPDATER__CONFIG_H
