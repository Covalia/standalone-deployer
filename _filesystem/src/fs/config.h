#ifndef FILESYSTEM__CONFIG_H
#define FILESYSTEM__CONFIG_H

#include <QString>

class FileSystemConfig
{
public:
    /// relative path of application installation application
    static const QString AppDir;

    /// relative path of application configuration
    static const QString ConfigurationDir;

    /// Relative path of application extensions
    static const QString ExtensionDir;

    /// Relative path of Image
    static const QString ImagesDir;

    /// Relative path of Java
    static const QString JavaDir;

    /// Relative path of the application logs
    static const QString LogsDir;

    /// Relative path of the temporary directory of the application
    static const QString TempDir;

    /// Relative directory path of application updater
    static const QString UpdaterDir;

    /// Loader file name
    static const QString LoaderFile;

    /// Updater Loader file name
    static const QString UpdaterFile;

    /// Name of executables windows extension
    static const QString WindowsExtension;

    /// Name of executables Mac OS extension
    static const QString MacOSExtension;

    /// Name of configuration file
    static const QString ConfigurationFile;

};

#endif // ifndef FILESYSTEM__CONFIG_H
