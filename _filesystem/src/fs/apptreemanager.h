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

        QDir getAppDirPath();
        QDir getConfigurationDirPath();
        QDir getExtensionDirPath();
        QDir getJavaDirPath();
        QDir getLogsDirPath();
        QDir getTempDirPath();
        QDir getUpdaterDirPath();

    private:
        static bool makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir);

        const QDir m_installationDir;
};

#endif // ifndef FILESYSTEM__APPTREEMANAGER_H
