#ifndef IO__INSTALLPATHIMPL_H
#define IO__INSTALLPATHIMPL_H

#include "tools/installpath/installpath.h"
#include "io/config.h"

#include <QCoreApplication>
#include <QDir>

class InstallPathImpl {
    public:

        static const QString ResourceBinPrefix;

        // Tout le code en commun vient ici
        InstallPathImpl(FileSystemConfig::AppComponent _app);
        virtual ~InstallPathImpl();
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

        virtual QPair<bool, QString> extractResourceToPath(QString resourcePath, QString copyFilePath);

        virtual bool makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir);

    protected:
        QDir m_installationDir;
        bool cdUp(QDir &_dir, int _numUp);
};

#endif
