#ifndef IO__INSTALLPATH_H
#define IO__INSTALLPATH_H

#include <QDir>
#include "tools/installpath/installpathimpl.h"
#include "io/config.h"

class InstallPathImpl;

class InstallPath {
    public:
        InstallPath(FileSystemConfig::AppComponent _app);
        virtual ~InstallPath();

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
        InstallPathImpl * m_pathInstallImpl;
};

#endif
