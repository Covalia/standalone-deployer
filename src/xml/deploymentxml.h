#ifndef DEPLOYMENTXML_H
#define DEPLOYMENTXML_H

#include <QString>
#include <QXmlStreamReader>
#include <QList>
#include <QMap>
#include <QFile>
#include <QDebug>
#include "xml/download.h"
#include "app/application.h"
#include "app/javaupdate.h"

// TODO sortir les constantes.
static QString DEPLOYMENT_CNLP_FILE_EXTENSION = ".cnlp";

static  QString R_DEPLOYMENT_TAG = "deployment";
static  QString DOWNLOADS_TAG = "downloads";
static  QString JAR_TAG = "jar";

static  QString RESOURCE_TAG = "resource";

static  QString OS_ATTRIBUTE = "os";
// TODO struct ?
static  QString OS_MACOSX_VALUE = "MacOSX";
static  QString OS_WINDOWS_VALUE = "Windows";
static  QString OS_ANY_VALUE = "Any";
static  QString NATIVE_ATTRIBUTE = "native";
static  QString HREF_ATTRIBUTE = "href";
static  QString HASH_ATTRIBUTE = "hash";
static  QString MAIN_ATTRIBUTE = "main";

static  QString APPLICATION_TAG = "application";
static  QString FILE_TAG = "file";

static  QString VERSION_ATTRIBUTE = "version";
static  QString NAME_ATTRIBUTE = "name";

static  QString DOWNLOADER_EXTENSION_CLASSPATH_ATTRIBUTE = "downloaderExtensionClasspath";

static  QString ARGUMENTS_TAG = "arguments";
static  QString ARGUMENT_TAG = "argument";

static  QString MEMORY_TAG = "memory";
static  QString VERSION_TAG = "version";

static  QString JAVA_TAG = "java";
static  QString JAVA_VERSION_ATTRIBUTE = "version";


class DeploymentXML : public QObject
{
    Q_OBJECT

public:
    explicit DeploymentXML(const QString &_pathCnlp, QObject *_parent = 0);
    ~DeploymentXML();

    bool read();

    QString getVersion() const;
    QList<QString> getArguments() const;
    QMap<Application, QList<Download> > getApplications() const;
    QMap<QString, JavaUpdate> getJavaUpdates() const;
    QString getMemory() const;

private:
    bool processDeployment();
    bool processVersion();
    bool processMemory();
    bool processJava();
    bool processArguments();
    bool processArgument();
    bool processDownloads();
    bool processApplication();
    Download processDownload();

    QString readNextText();

    QFile m_xmlFile;
    QXmlStreamReader m_xmlReader;
    QString m_memory;
    QString m_version;
    QList<QString> m_arguments;
    QMap<Application,QList<Download>> m_applications;
    QMap<QString,JavaUpdate> m_javaUpdates;
};

#endif
