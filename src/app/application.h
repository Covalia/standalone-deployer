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
    QString name;
    QString version;
    QString cnlpRemoteFileName;
    QString cnlpLocalFileName;
    QString downloaderExtensionClasspath;

public:
    Application(QString _name,
                QString _cnlpRemoteFileName,
                QString _cnlpLocalFileName);

    //bool operator==(Application const& a);

    //bool operator<(Application const& a);


};

inline bool operator<(const Application &e1, const Application &e2)
{
    return e1.name < e2.name;
}

#endif // APPLICATION_H
