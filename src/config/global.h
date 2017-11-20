#ifndef CONFIG__GLOBAL_H
#define CONFIG__GLOBAL_H

#include <QString>

class Global
{
public:
    /// URL de l'application
    static const QString AppUrl;

    /// Chemin de l'installation
    static const QString InstallationDir;

    /// Chemin relatif d'installation de l'application
    static const QString AppDir;

    /// Chemin relatif de la configuration de l'application
    static const QString ConfigurationDir;

    /// Chemin relatif des extensions de l'application
    static const QString ExtensionDir;

    /// Chemin relatif de Java
    static const QString JavaDir;

    /// Chemin relatif des logs de l'application
    static const QString LogsDir;

    /// Chemin relatif du répertoire temp de l'application
    static const QString TempDir;

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

#endif // ifndef CONFIG__GLOBAL_H
