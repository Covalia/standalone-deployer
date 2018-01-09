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

    /// Nom application.
    static const QString AppName;
    /// Nom fichier application distant.
    static const QString AppCnlpRemoteFilename;
    /// Nom fichier application local.
    static const QString AppCnlpLocalFilename;

    /// Nom starter.
    static const QString StarterName;
    /// Nom fichier starter distant.
    static const QString StarterCnlpRemoteFilename;
    /// Nom fichier starter local.
    static const QString StarterCnlpLocalFilename;

    /// Nom updater.
    static const QString UpdaterName;
    /// Nom fichier updater distant.
    static const QString UpdaterCnlpRemoteFilename;
    /// Nom fichier updater local.
    static const QString UpdaterCnlpLocalFilename;
};

#endif // ifndef UPDATER__CONFIG_H
