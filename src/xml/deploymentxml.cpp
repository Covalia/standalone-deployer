#include "deploymentxml.h"
#include <QFile>


DeploymentXML::DeploymentXML(const QString _pathCnlp, QObject *_parent) :
    QObject(_parent)
{
    this->m_memory = "512";
    this->m_version = "";
    this->m_applications = QMap<Application,QList<Download>>();
    this->m_javaUpdates = QMap<QString, JavaUpdate>();
    this->m_arguments = QList<QString>();

    QFile xmlFile(_pathCnlp);

    if(!xmlFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Cannot read file" << xmlFile.errorString();
        return;
    }

    m_xml.setDevice(&xmlFile);

    if (m_xml.readNextStartElement() && m_xml.name() == "deployment"){
        processDeployment();
    }
}

DeploymentXML::~DeploymentXML()
{
}

QString DeploymentXML::readNextText() {
#ifndef USE_READ_ELEMENT_TEXT
    m_xml.readNext();
    return m_xml.text().toString();
#else
    return xml.readElementText();
#endif
}

QMap<QString, JavaUpdate> DeploymentXML::getJavaUpdates() const
{
    return m_javaUpdates;
}

QMap<Application, QList<Download> > DeploymentXML::getApplications() const
{
    return m_applications;
}

QString DeploymentXML::getMemory() const
{
    return m_memory;
}

QList<QString> DeploymentXML::getArguments() const
{
    return m_arguments;
}

QString DeploymentXML::getVersion() const
{
    return m_version;
}

void DeploymentXML::processDeployment() {
    if (!m_xml.isStartElement() || m_xml.name() != R_DEPLOYMENT_TAG){
        return;
    }
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == VERSION_TAG){
            processVersion();
            m_xml.skipCurrentElement();
        }
        else if (m_xml.name() == MEMORY_TAG){
            processMemory();
            m_xml.skipCurrentElement();
        }
        else  if (m_xml.name() == JAVA_TAG){
            processJava();
            m_xml.skipCurrentElement();
        }
        else  if (m_xml.name() == ARGUMENTS_TAG){
            processArguments();
        }
        else  if (m_xml.name() == DOWNLOADS_TAG){
            processDownloads();
        }
        else{
            m_xml.skipCurrentElement();
        }
    }
}

void DeploymentXML::processVersion() {
    if (!m_xml.isStartElement() || m_xml.name() != VERSION_TAG){
        return;
    }
    m_version = readNextText();
}

void DeploymentXML::processMemory() {
    if (!m_xml.isStartElement() || m_xml.name() != MEMORY_TAG){
        return;
    }
    m_memory = readNextText();
}

void DeploymentXML::processJava() {
    if (!m_xml.isStartElement() || m_xml.name() != JAVA_TAG){
        return;
    }

    QString javaVersion = "";
    QString javaFile = "";
    QString javaHash = "";
    QString javaOs = "";

    if(m_xml.attributes().hasAttribute(JAVA_VERSION_ATTRIBUTE)){
        javaVersion = m_xml.attributes().value(JAVA_VERSION_ATTRIBUTE).toString();
    }
    if(m_xml.attributes().hasAttribute(HREF_ATTRIBUTE)){
        javaFile = m_xml.attributes().value(HREF_ATTRIBUTE).toString();
    }
    if(m_xml.attributes().hasAttribute(HASH_ATTRIBUTE)){
        javaHash = m_xml.attributes().value(HASH_ATTRIBUTE).toString();
    }
    if(m_xml.attributes().hasAttribute(OS_ATTRIBUTE)){
        javaOs = m_xml.attributes().value(OS_ATTRIBUTE).toString();
    }

    if(!javaVersion.isEmpty() && !javaFile.isEmpty() && !javaHash.isEmpty() && !javaOs.isEmpty()){
        JavaUpdate ju(javaVersion, javaFile, javaHash);
        m_javaUpdates.insert(javaOs, ju);
    }
}

void DeploymentXML::processArguments() {
    if (!m_xml.isStartElement() || m_xml.name() != ARGUMENTS_TAG){
        return;
    }
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == ARGUMENT_TAG){
            processArgument();
        }
        m_xml.skipCurrentElement();
    }
}

void DeploymentXML::processArgument() {
    if (!m_xml.isStartElement() || m_xml.name() != ARGUMENT_TAG){
        return;
    }
    QString arg = readNextText();
    if(arg != "null"){
        m_arguments << arg;
    }
}

void DeploymentXML::processDownloads() {
    if (!m_xml.isStartElement() || m_xml.name() != DOWNLOADS_TAG){
        return;
    }
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == APPLICATION_TAG){
            processApplication();
        }
        m_xml.skipCurrentElement();
    }
}

void DeploymentXML::processApplication() {
    if (!m_xml.isStartElement() || m_xml.name() != APPLICATION_TAG){
        return;
    }

    //Application application("","","");
    Application *application = 0;

    QString name = "";
    QString version = "";
    QString downloaderExtensionClasspath = "";

    if(m_xml.attributes().hasAttribute(NAME_ATTRIBUTE)){
        name = m_xml.attributes().value(NAME_ATTRIBUTE).toString();
    }
    if(m_xml.attributes().hasAttribute(VERSION_ATTRIBUTE)){
        version = m_xml.attributes().value(VERSION_ATTRIBUTE).toString();
    }
    if(m_xml.attributes().hasAttribute(DOWNLOADER_EXTENSION_CLASSPATH_ATTRIBUTE)){
        downloaderExtensionClasspath = m_xml.attributes().value(DOWNLOADER_EXTENSION_CLASSPATH_ATTRIBUTE).toString();
    }

    if(name == NAME_FILE_APPLICATION){
        application = new Application(NAME_FILE_APPLICATION,REMOTE_FILE_APPLICATION,LOCAL_FILE_APPLICATION);
    }
    else if(name ==  NAME_FILE_STARTER){
        application = new Application(NAME_FILE_STARTER,REMOTE_FILE_STARTER, LOCAL_FILE_STARTER);
    }
    else if(name ==  NAME_FILE_DOWNLOADER){
        application = new Application(NAME_FILE_DOWNLOADER, REMOTE_FILE_DOWNLOADER, LOCAL_FILE_DOWNLOADER);
    }

    if(application){
        application->setVersion(version);
        application->setDownloaderExtensionClasspath(downloaderExtensionClasspath);

        QList<Download> downloads;

        while (m_xml.readNextStartElement()) {
            if ((m_xml.name() == JAR_TAG) || (m_xml.name() == FILE_TAG)){
                Download download = processDownload();
                downloads << download;
            }
            m_xml.skipCurrentElement();
        }
        m_applications.insert(*application, downloads);
        delete application;
    }
}

Download DeploymentXML::processDownload() {

    QString href = "";
    QString hashMac = "";
    QString os = OS_ANY_VALUE;
    bool native = false;
    bool main = false;

    if(m_xml.attributes().hasAttribute(HREF_ATTRIBUTE)){
        href = m_xml.attributes().value(HREF_ATTRIBUTE).toString();
    }
    if(m_xml.attributes().hasAttribute(HASH_ATTRIBUTE)){
        hashMac = m_xml.attributes().value(HASH_ATTRIBUTE).toString();
    }
    if(m_xml.attributes().hasAttribute(OS_ATTRIBUTE)){
        QString osValue = m_xml.attributes().value(OS_ATTRIBUTE).toString();
        if(osValue == OS_WINDOWS_VALUE){
            os = OS_WINDOWS_VALUE;
        }
        else if(osValue == OS_MACOSX_VALUE){
            os = OS_MACOSX_VALUE;
        }
    }
    if(m_xml.attributes().hasAttribute(NATIVE_ATTRIBUTE) && m_xml.attributes().value(NATIVE_ATTRIBUTE).toString() == "true"){
        native = true;
    }
    if(m_xml.attributes().hasAttribute(MAIN_ATTRIBUTE) && m_xml.attributes().value(MAIN_ATTRIBUTE).toString() == "true"){
        main = true;
    }

    Download app(href, hashMac, os, native, main);

    return app;
}

