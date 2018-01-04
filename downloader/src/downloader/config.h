#ifndef DOWNLOADER__CONFIG_H
#define DOWNLOADER__CONFIG_H

#include <QString>

class DownloaderConfig
{
public:
    // TODO lire depuis la config.ini
    /// URL de l'application
    static const QString AppUrl;

    // TODO inutile
    /// Chemin de l'installation
    static const QString InstallationDir;

    /// User-Agent du downloader
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

    /// Nom downloader.
    static const QString DownloaderName;
    /// Nom fichier downloader distant.
    static const QString DownloaderCnlpRemoteFilename;
    /// Nom fichier downloader local.
    static const QString DownloaderCnlpLocalFilename;
};

#endif // ifndef DOWNLOADER__CONFIG_H
