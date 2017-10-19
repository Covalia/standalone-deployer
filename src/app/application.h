#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>


static QString NAME_FILE_APPLICATION = "application";
static QString REMOTE_FILE_APPLICATION= "application.cnlp.php";
static QString LOCAL_FILE_APPLICATION= "application.cnlp";

static QString NAME_FILE_STARTER= "starter";
static QString REMOTE_FILE_STARTER= "starter.cnlp.php";
static QString LOCAL_FILE_STARTER= "starter.cnlp";

static QString NAME_FILE_DOWNLOADER= "downloader";
static QString REMOTE_FILE_DOWNLOADER= "downloader.cnlp.php";
static QString LOCAL_FILE_DOWNLOADER= "downloader.cnlp";

class Application
{ 

public:
    Application(QString _name,
                QString _cnlpRemoteFileName,
                QString _cnlpLocalFileName);

    QString getName() const;
    QString getVersion() const;
    QString getCnlpRemoteFileName() const;
    QString getCnlpLocalFileName() const;
    QString getDownloaderExtensionClasspath() const;

    void setName(const QString &name);
    void setVersion(const QString &version);
    void setCnlpRemoteFileName(const QString &cnlpRemoteFileName);
    void setCnlpLocalFileName(const QString &cnlpLocalFileName);
    void setDownloaderExtensionClasspath(const QString &downloaderExtensionClasspath);

private:
    QString m_name;
    QString m_version;
    QString m_cnlpRemoteFileName;
    QString m_cnlpLocalFileName;
    QString m_downloaderExtensionClasspath;
};

inline bool operator<(const Application &e1, const Application &e2)
{
    return e1.getName() < e2.getName();
}

#endif // APPLICATION_H
