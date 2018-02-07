#ifndef IO__APPPATH_H
#define IO__APPPATH_H

#include <QDir>
#include <QSharedPointer>
#include "factories/apppath/apppathimpl.h"
#include "io/config.h"

class AppPathImpl;

class AppPath {
    public:
        AppPath(FileSystemConfig::AppComponent _app);
        virtual ~AppPath();

        QDir getInstallationRootPath();
        void setInstallationRootPath(QDir _path);

        bool createDirectoryIfNotExist();
        bool makeAppDirectories();

        QString getInstallerVersion();
        QString getUpdaterVersion();
        QString getLoaderVersion();

        QDir getAppDirPath();
        QDir getConfigurationDirPath();
        QDir getExtensionDirPath();
        QDir getImagesDirPath();
        QDir getSlidesDirPath();
        QDir getJavaDirPath();
        QDir getLogsDirPath();
        QDir getTempDirPath();
        QDir getUpdaterDirPath();
        QDir getLoaderDirPath();

        QString getLoaderResourcesPath();
        QString getUpdaterResourcesPath();

        QString getLoaderFilePath();
        QString getUpdaterFilePath(QString updaterVersion);
        QString getConfigurationFilePath();

        QPair<bool, QString> extractResourceToPath(QString resourcePath, QString copyFilePath);

        bool makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir);

    private:
        QSharedPointer<AppPathImpl> m_appPathImpl;
};

#endif
