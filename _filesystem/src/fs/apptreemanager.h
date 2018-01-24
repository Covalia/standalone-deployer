#ifndef FILESYSTEM__APPTREEMANAGER_H
#define FILESYSTEM__APPTREEMANAGER_H

#include <QObject>
#include <QDir>

class AppTreeManager : public QObject
{
    Q_OBJECT

    public:
        AppTreeManager(const QDir &_installationDir, QObject * _parent = 0);
        virtual ~AppTreeManager();

        bool createDirectoryIfNotExist();
        bool makeAppDirectories();

        static QString getInstallerVersion();
        static QString getUpdaterVersion();
        static QString getLoaderVersion();

        QDir getAppDirPath();
        QDir getConfigurationDirPath();
        QDir getExtensionDirPath();
        QDir getImagesDirPath();
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

        static bool makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir);

    private:
        static QString getExtension();

        const QDir m_installationDir;

};

#endif // ifndef FILESYSTEM__APPTREEMANAGER_H
