#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <QString>

class IOConfig
{
    public:

        enum AppComponent {
            Loader,
            Updater,
            Installer
        };

        /// Loader application name.
        static const QString LoaderName;

        /// Updater application name.
        static const QString UpdaterName;

        /// Application application name.
        static const QString AppName;

        /// Cnlp application name.
        static const QString CnlpName;

        /// Java application name.
        static const QString JavaName;

        /// Relative path of application installation application
        static const QString AppDir;

        /// Relative path of application configuration
        static const QString ConfigurationDir;

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

        /// Loader filename
        static const QString LoaderFile;

        /// Updater filename
        static const QString UpdaterFile;

        /// Executables windows extension
        static const QString WindowsAppExtension;

        /// Executables Mac OS extension
        static const QString MacOsAppExtension;

        /// DMG images Mac OS extension
        static const QString MacOsDmgExtension;

        /// Name of configuration file
        static const QString ConfigurationFile;

        /// Locales
        static const QString LocaleFrFr;
        static const QString LocaleEnUs;

        /// Source code URL repository
        static const QString SourceCodeUrl;

        /// Network Timeout in ms
        static const int TimeOut = 10000;
};

#endif
