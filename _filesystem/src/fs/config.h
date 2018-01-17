#ifndef FILESYSTEM__CONFIG_H
#define FILESYSTEM__CONFIG_H

#include <QString>

class FileSystemConfig
{
public:
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

    /// Chemin relatif du répertoire temp de l'application
    static const QString UpdaterDir;

    /// Nom du fichier loader
    static const QString LoaderFile;

    /// Nom du fichier updater
    static const QString UpdaterFile;

    /// Nom de l'extension windows des executable
    static const QString WindowsExtension;

    /// Nom de l'extension macos des executable
    static const QString MacOSExtension;

};

#endif // ifndef FILESYSTEM__CONFIG_H
