#ifndef IO_APPPATHIMPL_H
#define IO_APPPATHIMPL_H

#include "factories/apppath/apppath.h"
#include "io/config.h"
#include "log/logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QProcess>

class AppPathImpl {
    public:

        /// Java sub directory name.
        static const QString JavaSubDirName;

        /// Application natives directory name
        static const QString NativesDirName;

        static const QString ResourceBinPrefix;

        // Tout le code en commun vient ici
        AppPathImpl(IOConfig::AppComponent _app);
        virtual ~AppPathImpl();
        virtual QDir getInstallationDir() const;
        // set install root path, call only from installer
        virtual void setInstallationDir(QDir _path);

        virtual bool createDirectoryIfNotExist();
        virtual bool makeAppDirectories() = 0;

        virtual QString getInstallerVersion();
        virtual QString getUpdaterVersion();
        virtual QString getLoaderVersion();

        virtual QDir getAppDir() const;
        virtual QDir getAppOldDir() const;
        virtual QDir getAppNativesDir() const;
        virtual QDir getConfigurationDir() const;
        virtual QDir getImagesDir() const;
        virtual QDir getSlidesDir() const;
        virtual QDir getJavaDir() const;
        virtual QDir getJavaVersionDir(const QString &_version) const;
        virtual QDir getJavaDistDir(const QString &_version) const;
        virtual QDir getLogsDir() const;
        virtual QDir getTempDir() const;
        virtual QDir getTempAppDir() const;
        virtual QDir getTempAppBuildDir() const;
        virtual QDir getTempCnlpDir() const;
        virtual QDir getTempJavaDir() const;
        virtual QDir getTempJavaBuildDir() const;
        virtual QDir getTempLoaderDir() const;
        virtual QDir getTempLoaderBuildDir() const;
        virtual QDir getTempUpdaterDir() const;
        virtual QDir getTempUpdaterBuildDir() const;
        virtual QDir getTempDataBuildDir() const;
        virtual QDir getCnlpDir() const;
        virtual QDir getCnlpOldDir() const;
        virtual QDir getDataDir() const;

        virtual QDir getUpdaterDir() const;
        virtual QDir getUpdaterVersionDir(const QString &_version) const;
        virtual QDir getLoaderDir() const;
        virtual QDir getLoaderOldDir() const;

        virtual QSharedPointer<QFile> getLoaderResourcesFile() = 0;
        virtual QSharedPointer<QFile> getUpdaterResourcesFile() = 0;

        virtual QSharedPointer<QFile> getLoaderFile() = 0;
        virtual QSharedPointer<QFile> getUpdaterFile(QString updaterVersion) = 0;

        virtual QSharedPointer<QFile> getConfigurationFile();

        bool cleanAppDir();
        bool cleanCnlpDir();
        bool cleanImagesDir();
        bool cleanJavaDir();
        bool cleanLoaderDir();
        bool cleanTempDir();
        bool cleanUpdaterDir();

        bool cleanUpdaterDirExceptVersion(const QString &_version);
        bool cleanJavaDirExceptVersion(const QString &_version);

        virtual bool prepareLoader();
        virtual bool prepareUpdater(QString _version);
        virtual bool prepareJava(const QString &_version, bool _forceOverwrite);

        virtual bool startLoader(QStringList _args) = 0;
        virtual bool startUpdater(QString _version, QStringList _args) = 0;

        virtual QPair<bool, QString> extractResource(QFile &_sourceFile, QFile &_destFile);

        virtual bool makeDirectoryIfNotExisting(const QDir &_directory);

        virtual bool startApplication(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                                      const QString &_mainClass, const QString &_encoding, const QString &_dataLocation,
                                      const QStringList &_arguments);

        virtual bool startPostInstallTasks(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                                           const QString &_runnerClass, const QString &_encoding, const QString &_dataLocation);

        virtual QString getClasspathSeparator() const = 0;

        static QString getDataPathFromInstallPath(const QString &_installPath);

    protected:

        /// Build directory suffix.
        static const QString BuildDirSuffix;
        /// Old directory suffix.
        static const QString OldDirSuffix;

        QDir m_installationDir;
        bool cdUp(QDir &_dir, int _numUp);

        virtual QString getJavaExecutablePath(const QString &_javaVersion) const = 0;

        bool cleanDir(const QString &_folder);
        bool cleanExceptVersion(const QDir &_dir, const QString &_version);
};

#endif
