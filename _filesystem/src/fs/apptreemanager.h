#ifndef DOWNLOADER__APPTREEMANAGER_H
#define DOWNLOADER__APPTREEMANAGER_H

#include <QObject>
#include <QDir>

class AppTreeManager : public QObject
{
    Q_OBJECT

public:
    AppTreeManager(const QDir &_installationDir, QObject * _parent = 0);
    virtual ~AppTreeManager();

    bool makeAppDirectories();

    QDir getAppDirPath();
    QDir getConfigurationDirPath();
    QDir getExtensionDirPath();
    QDir getJavaDirPath();
    QDir getLogsDirPath();
    QDir getTempDirPath();

private:
    static bool makeDirectoryIfNotExists(QDir _directoryPath, const QString &_subDir);

    const QDir m_installationDir;
};

#endif // ifndef DOWNLOADER__APPTREEMANAGER_H