#ifndef DEPLOYMENTXML_H
#define DEPLOYMENTXML_H

#include <iostream>
#include <string>

#include <QXmlStreamReader>
#include <QString>
#include <QFile>
#include <QList>
#include <QMap>
#include <QListIterator>
#include <QDebug>

#include "download.h"
#include "../app/application.h"
#include "../app/javaupdate.h"

static QString DEPLOYMENT_CNLP_FILE_EXTENSION = ".cnlp";

static  QString R_DEPLOYMENT_TAG = "deployment";
static  QString DOWNLOADS_TAG = "downloads";
static  QString JAR_TAG = "jar";

static  QString RESOURCE_TAG = "resource";

static  QString OS_ATTRIBUTE = "os";
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


class DeploymentXML
{
public:
    QString memory;
    QString version;
    QList<QString> arguments;
    QMap<Application,QList<Download>> applications;
    QMap<QString,JavaUpdate> javaUpdates;

public:
    DeploymentXML(const QString pathCnlp);

private:
    void processDeployment();
    void processVersion();
    void processMemory();
    void processJava();
    void processArguments();
    void processArgument();
    void processDownloads();
    void processApplication();
    Download processDownload();

    QString readNextText();
    QXmlStreamReader xml;
};

#endif // DEPLOYMENTXML_H
