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

        QDir getInstallationDir();
        void setInstallationDir(QDir _path);

        bool createDirectoryIfNotExist();
        bool makeAppDirectories();

        QString getInstallerVersion();
        QString getUpdaterVersion();
        QString getLoaderVersion();

        QDir getAppDir();
        QDir getAppOldDir();
        QDir getConfigurationDir();
        QDir getExtensionDir();
        QDir getImagesDir();
        QDir getSlidesDir();
        QDir getJavaDir();
        QDir getLogsDir();
        QDir getTempDir();
        QDir getTempAppDir();
        QDir getTempAppBuildDir();
        QDir getTempCnlpDir();
        QDir getTempJavaDir();
        QDir getTempJavaBuildDir();
        QDir getTempLoaderDir();
        QDir getTempLoaderBuildDir();
        QDir getTempUpdaterDir();
        QDir getTempUpdaterBuildDir();
        QDir getCnlpDir();
        QDir getCnlpOldDir();
        QDir getDataDir();

        QDir getUpdaterDir();
        QDir getLoaderDir();
        QDir getLoaderOldDir();

        QSharedPointer<QFile> getLoaderResourcesFile();
        QSharedPointer<QFile> getUpdaterResourcesFile();

        QSharedPointer<QFile> getLoaderFile();
        QSharedPointer<QFile> getUpdaterFile(QString updaterVersion);
        QSharedPointer<QFile> getConfigurationFile();

        // will extract app from dmg on macos, nothing on windows
        bool prepareLoader();
        bool prepareUpdater(QString _version);
        bool prepareJava(const QString &_version, bool _forceOverwrite);

        bool startLoader(QStringList _args);
        bool startUpdater(QString _version, QStringList _args);

        QPair<bool, QString> extractResource(QFile &_sourceFile, QFile &_destFile);

        bool makeDirectoryIfNotExists(QDir _directory, const QString &_subDir);

    private:
        QSharedPointer<AppPathImpl> m_appPathImpl;
};

#endif
