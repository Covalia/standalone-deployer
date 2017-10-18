#include "deploymentxml.h"


DeploymentXML::DeploymentXML(const QString _pathCnlp)
{
    this->memory = "512";
    this->version = "";
    this->applications = QMap<Application,QList<Download>>();
    this->javaUpdates = QMap<QString, JavaUpdate>();
    this->arguments = QList<QString>();

    QFile xmlFile(_pathCnlp);

    if(!xmlFile.open(QFile::ReadOnly | QFile::Text)){
            qDebug() << "Cannot read file" << xmlFile.errorString();
            return;
    }

    xml.setDevice(&xmlFile);

    if (xml.readNextStartElement() && xml.name() == "deployment"){
        processDeployment();
    }
}


QString DeploymentXML::readNextText() {
#ifndef USE_READ_ELEMENT_TEXT
    xml.readNext();
    return xml.text().toString();
#else
    return xml.readElementText();
#endif
}

void DeploymentXML::processDeployment() {
    if (!xml.isStartElement() || xml.name() != R_DEPLOYMENT_TAG)
        return;
    while (xml.readNextStartElement()) {
        if (xml.name() == VERSION_TAG){
            processVersion();
            xml.skipCurrentElement();
        }
        else if (xml.name() == MEMORY_TAG){
            processMemory();
            xml.skipCurrentElement();
        }
        else  if (xml.name() == JAVA_TAG){
            processJava();
            xml.skipCurrentElement();
        }
        else  if (xml.name() == ARGUMENTS_TAG){
            processArguments();
        }
        else  if (xml.name() == DOWNLOADS_TAG){
            processDownloads();
        }
        else{
            xml.skipCurrentElement();
        }
    }
}

void DeploymentXML::processVersion() {
    if (!xml.isStartElement() || xml.name() != VERSION_TAG)
        return;
    version = readNextText();
}

void DeploymentXML::processMemory() {
    if (!xml.isStartElement() || xml.name() != MEMORY_TAG)
        return;
    memory = readNextText();
}

void DeploymentXML::processJava() {
    if (!xml.isStartElement() || xml.name() != JAVA_TAG)
        return;

    QString javaVersion = "";
    QString javaFile = "";
    QString javaHash = "";
    QString javaOs = "";

    if(xml.attributes().hasAttribute(JAVA_VERSION_ATTRIBUTE)){
        javaVersion = xml.attributes().value(JAVA_VERSION_ATTRIBUTE).toString();
    }
    if(xml.attributes().hasAttribute(HREF_ATTRIBUTE)){
        javaFile = xml.attributes().value(HREF_ATTRIBUTE).toString();
    }
    if(xml.attributes().hasAttribute(HASH_ATTRIBUTE)){
        javaHash = xml.attributes().value(HASH_ATTRIBUTE).toString();
    }
    if(xml.attributes().hasAttribute(OS_ATTRIBUTE)){
        javaOs = xml.attributes().value(OS_ATTRIBUTE).toString();
    }

    if(!javaVersion.isEmpty() && !javaFile.isEmpty() && !javaHash.isEmpty() && !javaOs.isEmpty()){
        JavaUpdate ju(javaVersion, javaFile, javaHash);
        javaUpdates.insert(javaOs, ju);
    }
}

void DeploymentXML::processArguments() {
    if (!xml.isStartElement() || xml.name() != ARGUMENTS_TAG)
        return;
    while (xml.readNextStartElement()) {
        if (xml.name() == ARGUMENT_TAG){
            processArgument();
        }
        xml.skipCurrentElement();
    }
}

void DeploymentXML::processArgument() {
    if (!xml.isStartElement() || xml.name() != ARGUMENT_TAG)
        return;
    QString arg = readNextText();
    if(arg != "null"){
        arguments << arg;
    }
}

void DeploymentXML::processDownloads() {
    if (!xml.isStartElement() || xml.name() != DOWNLOADS_TAG)
        return;
    while (xml.readNextStartElement()) {
        if (xml.name() == APPLICATION_TAG){
            processApplication();
        }
        xml.skipCurrentElement();
    }
}

void DeploymentXML::processApplication() {
    if (!xml.isStartElement() || xml.name() != APPLICATION_TAG)
        return;

    Application application("","","");

    QString name = "";
    QString version = "";
    QString downloaderExtensionClasspath = "";

    if(xml.attributes().hasAttribute(NAME_ATTRIBUTE))
        name = xml.attributes().value(NAME_ATTRIBUTE).toString();
    if(xml.attributes().hasAttribute(VERSION_ATTRIBUTE))
        version = xml.attributes().value(VERSION_ATTRIBUTE).toString();
    if(xml.attributes().hasAttribute(DOWNLOADER_EXTENSION_CLASSPATH_ATTRIBUTE))
        downloaderExtensionClasspath = xml.attributes().value(DOWNLOADER_EXTENSION_CLASSPATH_ATTRIBUTE).toString();

    if(name == NAME_FILE_APPLICATION){
        application = Application(NAME_FILE_APPLICATION,REMOTE_FILE_APPLICATION,LOCAL_FILE_APPLICATION);
    }
    else if(name ==  NAME_FILE_STARTER){
        application = Application(NAME_FILE_STARTER,REMOTE_FILE_STARTER, LOCAL_FILE_STARTER);
    }
    else if(name ==  NAME_FILE_DOWNLOADER){
        application = Application(NAME_FILE_DOWNLOADER, REMOTE_FILE_DOWNLOADER, LOCAL_FILE_DOWNLOADER);
    }

    if(application.name != ""){
        application.version = version;
        application.downloaderExtensionClasspath = downloaderExtensionClasspath;

        QList<Download> downloads;

        while (xml.readNextStartElement()) {
            if ((xml.name() == JAR_TAG) || (xml.name() == FILE_TAG)){
                Download download = processDownload();
                downloads << download;
            }
            xml.skipCurrentElement();
        }
        applications.insert(application, downloads);
    }
}

Download DeploymentXML::processDownload() {

    QString href = "";
    QString hashMac = "";
    QString os = OS_ANY_VALUE;
    bool native = false;
    bool main = false;

    if(xml.attributes().hasAttribute(HREF_ATTRIBUTE)){
        href = xml.attributes().value(HREF_ATTRIBUTE).toString();
    }
    if(xml.attributes().hasAttribute(HASH_ATTRIBUTE)){
        hashMac = xml.attributes().value(HASH_ATTRIBUTE).toString();
    }
    if(xml.attributes().hasAttribute(OS_ATTRIBUTE)){
        QString osValue = xml.attributes().value(OS_ATTRIBUTE).toString();
        if(osValue == OS_WINDOWS_VALUE){
            os = OS_WINDOWS_VALUE;
        }
        else if(osValue == OS_MACOSX_VALUE){
            os = OS_MACOSX_VALUE;
        }
    }
    if(xml.attributes().hasAttribute(NATIVE_ATTRIBUTE) && xml.attributes().value(NATIVE_ATTRIBUTE).toString() == "true"){
        native = true;
    }
    if(xml.attributes().hasAttribute(MAIN_ATTRIBUTE) && xml.attributes().value(MAIN_ATTRIBUTE).toString() == "true"){
        main = true;
    }

    Download app(href, hashMac, os, native, main);

    return app;
}
