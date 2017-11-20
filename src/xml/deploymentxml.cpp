#include "xml/deploymentxml.h"
#include "config/global.h"
#include <QFile>

DeploymentXML::DeploymentXML(const QString &_pathCnlp, QObject * _parent) :
    QObject(_parent),
    m_xmlFile(_pathCnlp)
{
    m_memory = "512";
    m_version = "";
    m_applications = QMap<Application, QList<Download> >();
    m_javaUpdates = QMap<QString, JavaUpdate>();
    m_arguments = QList<QString>();
}

DeploymentXML::~DeploymentXML()
{
}

bool DeploymentXML::read()
{
    if (!m_xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Cannot read file" << m_xmlFile.errorString();
        return false;
    }

    m_xmlReader.setDevice(&m_xmlFile);

    // TODO vérifier qu'on traite bien les erreurs XML
    bool result = processDeployment();

    m_xmlFile.close();

    return result;
}

QString DeploymentXML::readNextText()
{
    m_xmlReader.readNext();
    return m_xmlReader.text().toString();
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

bool DeploymentXML::processDeployment()
{
    if (!m_xmlReader.readNextStartElement()) {
        return false;
    }

    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != R_DEPLOYMENT_TAG) {
        return false;
    }

    bool result = true;

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == VERSION_TAG) {
            result &= processVersion();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == MEMORY_TAG) {
            result &= processMemory();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == JAVA_TAG) {
            result &= processJava();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == ARGUMENTS_TAG) {
            result &= processArguments();
        } else if (m_xmlReader.name() == DOWNLOADS_TAG) {
            result &= processDownloads();
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }

    return result;
} // DeploymentXML::processDeployment

bool DeploymentXML::processVersion()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != VERSION_TAG) {
        return false;
    }
    m_version = readNextText();
    return true;
}

bool DeploymentXML::processMemory()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != MEMORY_TAG) {
        return false;
    }
    m_memory = readNextText();
    return true;
}

bool DeploymentXML::processJava()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != JAVA_TAG) {
        return false;
    }

    QString javaVersion = "";
    QString javaFile = "";
    QString javaHash = "";
    QString javaOs = "";

    // TODO false si les attributs n'existent pas
    if (m_xmlReader.attributes().hasAttribute(JAVA_VERSION_ATTRIBUTE)) {
        javaVersion = m_xmlReader.attributes().value(JAVA_VERSION_ATTRIBUTE).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(HREF_ATTRIBUTE)) {
        javaFile = m_xmlReader.attributes().value(HREF_ATTRIBUTE).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(HASH_ATTRIBUTE)) {
        javaHash = m_xmlReader.attributes().value(HASH_ATTRIBUTE).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(OS_ATTRIBUTE)) {
        javaOs = m_xmlReader.attributes().value(OS_ATTRIBUTE).toString();
    }

    if (!javaVersion.isEmpty() && !javaFile.isEmpty() && !javaHash.isEmpty() && !javaOs.isEmpty()) {
        JavaUpdate ju(javaVersion, javaFile, javaHash);
        m_javaUpdates.insert(javaOs, ju);
    }

    return true;
} // DeploymentXML::processJava

bool DeploymentXML::processArguments()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != ARGUMENTS_TAG) {
        return false;
    }

    bool result = true;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == ARGUMENT_TAG) {
            result &= processArgument();
        }
        m_xmlReader.skipCurrentElement();
    }
    return result;
}

bool DeploymentXML::processArgument()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != ARGUMENT_TAG) {
        return false;
    }

    QString arg = readNextText();
    if (arg != "null") {
        m_arguments << arg;
    }
    return true;
}

bool DeploymentXML::processDownloads()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != DOWNLOADS_TAG) {
        return false;
    }

    bool result = true;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == APPLICATION_TAG) {
            result &= processApplication();
        }
        m_xmlReader.skipCurrentElement();
    }
    return result;
}

bool DeploymentXML::processApplication()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != APPLICATION_TAG) {
        return false;
    }

    Application * application = 0;

    QString name = "";
    QString version = "";
    QString downloaderExtensionClasspath = "";

    // TODO check attributes et return false
    if (m_xmlReader.attributes().hasAttribute(NAME_ATTRIBUTE)) {
        name = m_xmlReader.attributes().value(NAME_ATTRIBUTE).toString();
    }
    if (m_xmlReader.attributes().hasAttribute(VERSION_ATTRIBUTE)) {
        version = m_xmlReader.attributes().value(VERSION_ATTRIBUTE).toString();
    }
    if (m_xmlReader.attributes().hasAttribute(DOWNLOADER_EXTENSION_CLASSPATH_ATTRIBUTE)) {
        downloaderExtensionClasspath = m_xmlReader.attributes().value(DOWNLOADER_EXTENSION_CLASSPATH_ATTRIBUTE).toString();
    }

    if (name == Global::AppName) {
        application = new Application(Global::AppName, Global::AppCnlpRemoteFilename, Global::AppCnlpLocalFilename);
    } else if (name ==  Global::StarterName) {
        application = new Application(Global::StarterName, Global::StarterCnlpRemoteFilename, Global::StarterCnlpLocalFilename);
    } else if (name ==  Global::DownloaderName) {
        application = new Application(Global::DownloaderName, Global::DownloaderCnlpRemoteFilename, Global::StarterCnlpLocalFilename);
    }

    if (application) {
        application->setVersion(version);
        application->setDownloaderExtensionClasspath(downloaderExtensionClasspath);

        QList<Download> downloads;

        while (m_xmlReader.readNextStartElement()) {
            if ((m_xmlReader.name() == JAR_TAG) || (m_xmlReader.name() == FILE_TAG)) {
                Download download = processDownload();
                downloads << download;
            }
            m_xmlReader.skipCurrentElement();
        }
        m_applications.insert(*application, downloads);
        delete application;

        return true;
    }
    return false;
} // DeploymentXML::processApplication

Download DeploymentXML::processDownload()
{
    QString href = "";
    QString hashMac = "";
    QString os = OS_ANY_VALUE;
    bool native = false;
    bool main = false;

    if (m_xmlReader.attributes().hasAttribute(HREF_ATTRIBUTE)) {
        href = m_xmlReader.attributes().value(HREF_ATTRIBUTE).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(HASH_ATTRIBUTE)) {
        hashMac = m_xmlReader.attributes().value(HASH_ATTRIBUTE).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(OS_ATTRIBUTE)) {
        QString osValue = m_xmlReader.attributes().value(OS_ATTRIBUTE).toString();
        if (osValue == OS_WINDOWS_VALUE) {
            os = OS_WINDOWS_VALUE;
        } else if (osValue == OS_MACOSX_VALUE) {
            os = OS_MACOSX_VALUE;
        }
    }

    if (m_xmlReader.attributes().hasAttribute(NATIVE_ATTRIBUTE) && m_xmlReader.attributes().value(NATIVE_ATTRIBUTE).toString() == "true") {
        native = true;
    }

    if (m_xmlReader.attributes().hasAttribute(MAIN_ATTRIBUTE) && m_xmlReader.attributes().value(MAIN_ATTRIBUTE).toString() == "true") {
        main = true;
    }

    return Download(href, hashMac, os, native, main);
} // DeploymentXML::processDownload
