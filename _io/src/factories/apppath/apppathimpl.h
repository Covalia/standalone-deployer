#ifndef IO__APPPATHIMPL_H
#define IO__APPPATHIMPL_H

#include "factories/apppath/apppath.h"
#include "io/config.h"
#include "log/logger.h"

#include <QCoreApplication>
#include <QDir>
#include <QProcess>

class AppPathImpl {
    public:

        static const QString ResourceBinPrefix;

        // Tout le code en commun vient ici
        AppPathImpl(FileSystemConfig::AppComponent _app);
        virtual ~AppPathImpl();
        virtual QDir getInstallationRootPath();
        // set install root path, call only from installer
        virtual void setInstallationRootPath(QDir _path);

        virtual bool createDirectoryIfNotExist();
        virtual bool makeAppDirectories() = 0;

        virtual QString getInstallerVersion();
        virtual QString getUpdaterVersion();
        virtual QString getLoaderVersion();

        virtual QDir getAppDirPath();
        virtual QDir getConfigurationDirPath();
        virtual QDir getExtensionDirPath();
        virtual QDir getImagesDirPath();
        virtual QDir getSlidesDirPath();
        virtual QDir getJavaDirPath();
        virtual QDir getLogsDirPath();
        virtual QDir getTempDirPath();
        virtual QDir getUpdaterDirPath();
        virtual QDir getLoaderDirPath();

        virtual QString getLoaderResourcesPath() = 0;
        virtual QString getUpdaterResourcesPath() = 0;

        virtual QString getLoaderFilePath() = 0;
        virtual QString getUpdaterFilePath(QString updaterVersion) = 0;

        virtual QString getConfigurationFilePath();

        virtual bool startLoader(QStringList _args);
        virtual bool startUpdater(QString _version, QStringList _args);

        virtual QPair<bool, QString> extractResourceToPath(QString resourcePath, QString copyFilePath);

        virtual bool makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir);

    protected:
        QDir m_installationDir;
        virtual bool startApplication(QString _app, QStringList _args) = 0;
        bool cdUp(QDir &_dir, int _numUp);
};

#endif
