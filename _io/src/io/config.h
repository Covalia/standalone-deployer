#ifndef IO__CONFIG_H
#define IO__CONFIG_H

#include <QString>

class IOConfig
{
public:

    enum AppComponent {
        Loader,
        Updater,
        Installer
    };

    /// relative path of application installation application
    static const QString InstallerVersion;

    /// relative path of application configuration
    static const QString UpdateVersion;

    /// Relative path of application extensions
    static const QString LoaderVersion;


    /// Nom loader.
    static const QString LoaderName;

    /// Nom updater.
    static const QString UpdaterName;

    /// Nom application.
    static const QString AppName;

    /// Nom cnlp
    static const QString CnlpName;

    /// Java application name.
    static const QString JavaName;

    /// relative path of application installation application
    static const QString AppDir;

    /// relative path of application configuration
    static const QString ConfigurationDir;

    /// Relative path of application extensions
    static const QString ExtensionDir;

    /// Relative path of Image
    static const QString ImagesDir;

    /// Relative path of Slides
    static const QString SlidesDir;

    /// Relative path of Java
    static const QString JavaDir;

    /// Relative path of the application logs
    static const QString LogsDir;

    /// Relative path of the temporary directory of the application
    static const QString TempDir;

    /// Relative path of the Cnlp directory
    static const QString CnlpDir;

    /// Relative path of the mount directory of the application, macos only
    static const QString MountDir;

    /// Relative directory path of application updater
    static const QString UpdaterDir;

    /// Relative directory path of application updater
    static const QString LoaderDir;

    /// Relative directory path of application data
    static const QString DataDir;


    /// Loader file name
    static const QString LoaderFile;

    /// Updater Loader file name
    static const QString UpdaterFile;

    /// Executables windows extension
    static const QString WindowsAppExtension;

    /// Executables Mac OS extension
    static const QString MacOsAppExtension;

    /// DMG images Mac OS extension
    static const QString MacOsDmgExtension;

    /// Java sub directory name.
    static const QString JavaSubDirName;


    /// Name of configuration file
    static const QString ConfigurationFile;

};

#endif // ifndef IO__CONFIG_H
