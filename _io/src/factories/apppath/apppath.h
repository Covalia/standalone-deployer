#ifndef IO__APPPATH_H
#define IO__APPPATH_H

#include <QDir>
#include <QSharedPointer>
#include "factories/apppath/apppathimpl.h"
#include "io/config.h"

class AppPathImpl;

class AppPath {
    public:
        AppPath(IOConfig::AppComponent _app);
        virtual ~AppPath();

        QDir getInstallationDir() const;
        void setInstallationDir(QDir _path);

        bool createDirectoryIfNotExist();
        bool makeAppDirectories();

        QString getInstallerVersion();
        QString getUpdaterVersion();
        QString getLoaderVersion();

        QDir getAppDir() const;
        QDir getAppOldDir() const;
        QDir getAppNativesDir() const;
        QDir getConfigurationDir() const;
        QDir getImagesDir() const;
        QDir getSlidesDir() const;
        QDir getJavaDir() const;
        QDir getJavaVersionDir(const QString &_version) const;
        QDir getJavaDistDir(const QString &_version) const;
        QDir getLogsDir() const;
        QDir getTempDir() const;
        QDir getTempAppDir() const;
        QDir getTempAppBuildDir() const;
        QDir getTempCnlpDir() const;
        QDir getTempJavaDir() const;
        QDir getTempJavaBuildDir() const;
        QDir getTempLoaderDir() const;
        QDir getTempLoaderBuildDir() const;
        QDir getTempUpdaterDir() const;
        QDir getTempUpdaterBuildDir() const;
        QDir getTempDataBuildDir() const;
        QDir getCnlpDir() const;
        QDir getCnlpOldDir() const;
        QDir getDataDir() const;
        QDir getUpdaterDir() const;
        QDir getUpdaterVersionDir(const QString &_version) const;
        QDir getLoaderDir() const;
        QDir getLoaderOldDir() const;

        QSharedPointer<QFile> getLoaderResourcesFile();
        QSharedPointer<QFile> getUpdaterResourcesFile();

        QSharedPointer<QFile> getLoaderFile();
        QSharedPointer<QFile> getUpdaterFile(QString updaterVersion);
        QSharedPointer<QFile> getConfigurationFile();

        bool cleanAppDir();
        bool cleanCnlpDir();
        bool cleanImagesDir();
        bool cleanJavaDir();
        bool cleanLoaderDir();
        bool cleanTempDir();
        bool cleanUpdaterDir();

        bool cleanUpdaterDirExceptVersion(const QString &_version);
        bool cleanJavaDirExceptVersion(const QString &_version);

        // will extract app from dmg on macos, nothing on windows
        bool prepareLoader();
        bool prepareUpdater(QString _version);
        bool prepareJava(const QString &_version, bool _forceOverwrite);

        bool startLoader(QStringList _args);
        bool startUpdater(QString _version, QStringList _args);

        QPair<bool, QString> extractResource(QFile &_sourceFile, QFile &_destFile);

        bool makeDirectoryIfNotExisting(const QDir &_directory);

        bool startApplication(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                              const QString &_mainClass, const QString &_encoding, const QString &_dataLocation,
                              const QStringList &_arguments);

        virtual bool startPostInstallTasks(const QString &_javaVersion, const QString &_xmxMemory, const QString &_classPath,
                                           const QString &_runnerClass, const QString &_encoding, const QString &_dataLocation);

        QString getClasspathSeparator() const;

    private:
        QSharedPointer<AppPathImpl> m_appPathImpl;
};

#endif
