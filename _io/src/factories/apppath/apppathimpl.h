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
        AppPathImpl(IOConfig::AppComponent _app);
        virtual ~AppPathImpl();
        virtual QDir getInstallationDir();
        // set install root path, call only from installer
        virtual void setInstallationDir(QDir _path);

        virtual bool createDirectoryIfNotExist();
        virtual bool makeAppDirectories() = 0;

        virtual QString getInstallerVersion();
        virtual QString getUpdaterVersion();
        virtual QString getLoaderVersion();

        virtual QDir getAppDir();
        virtual QDir getConfigurationDir();
        virtual QDir getExtensionDir();
        virtual QDir getImagesDir();
        virtual QDir getSlidesDir();
        virtual QDir getJavaDir();
        virtual QDir getLogsDir();
        virtual QDir getTempDir();
        virtual QDir getDataDir();
        virtual QDir getUpdaterDir();
        virtual QDir getLoaderDir();

        virtual QSharedPointer<QFile> getLoaderResourcesFile() = 0;
        virtual QSharedPointer<QFile> getUpdaterResourcesFile() = 0;

        virtual QSharedPointer<QFile> getLoaderFile() = 0;
        virtual QSharedPointer<QFile> getUpdaterFile(QString updaterVersion) = 0;

        virtual QSharedPointer<QFile> getConfigurationFile();

        virtual bool prepareLoader();
        virtual bool prepareUpdater(QString _version);

        virtual bool startLoader(QStringList _args);
        virtual bool startUpdater(QString _version, QStringList _args);

        virtual QPair<bool, QString> extractResource(QFile &_sourceFile, QFile &_destFile);

        virtual bool makeDirectoryIfNotExists(QDir _directory, const QString &_subDir);

    protected:
        QDir m_installationDir;
        virtual bool startApplication(QSharedPointer<QFile> _app, QStringList _args) = 0;
        bool cdUp(QDir &_dir, int _numUp);
};

#endif
