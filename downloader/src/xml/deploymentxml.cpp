#include "xml/deploymentxml.h"
#include "config/global.h"
#include <QFile>

const QString DeploymentXML::RDeploymentTag("deployment");
const QString DeploymentXML::DownloadsTag("downloads");
const QString DeploymentXML::JarTag("jar");

const QString DeploymentXML::ResourceTag("resource");

const QString DeploymentXML::OsAttribute("os");
const QString DeploymentXML::OsMacOsValue("MacOSX");
const QString DeploymentXML::OsWindowsValue("Windows");
const QString DeploymentXML::OsAnyValue("Any");
const QString DeploymentXML::NativeAttribute("native");
const QString DeploymentXML::HrefAttribute("href");
const QString DeploymentXML::HashAttribute("hash");
const QString DeploymentXML::MainAttribute("main");

const QString DeploymentXML::ApplicationTag("application");
const QString DeploymentXML::FileTag("file");

const QString DeploymentXML::VersionAttribute("version");
const QString DeploymentXML::NameAttribute("name");

const QString DeploymentXML::DownloaderExtensionClasspathAttribute("downloaderExtensionClasspath");

const QString DeploymentXML::ArgumentsTag("arguments");
const QString DeploymentXML::ArgumentTag("argument");

const QString DeploymentXML::MemoryTag("memory");
const QString DeploymentXML::VersionTag("version");

const QString DeploymentXML::JavaTag("java");
const QString DeploymentXML::JavaVersionAttribute("version");

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

QList<Download> DeploymentXML::getDownloads(const Application &_application, const QString &_os) const
{
    QList<Download> downloads;

    foreach(Download download, m_applications.value(_application)) {
        if (download.getOs() == _os || download.getOs() == OsAnyValue) {
            downloads.append(download);
        }
    }

    return downloads;
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

    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != RDeploymentTag) {
        return false;
    }

    bool result = true;

    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == VersionTag) {
            result &= processVersion();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == MemoryTag) {
            result &= processMemory();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == JavaTag) {
            result &= processJava();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == ArgumentsTag) {
            result &= processArguments();
        } else if (m_xmlReader.name() == DownloadsTag) {
            result &= processDownloads();
        } else {
            m_xmlReader.skipCurrentElement();
        }
    }

    return result;
} // DeploymentXML::processDeployment

bool DeploymentXML::processVersion()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != VersionTag) {
        return false;
    }
    m_version = readNextText();
    return true;
}

bool DeploymentXML::processMemory()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != MemoryTag) {
        return false;
    }
    m_memory = readNextText();
    return true;
}

bool DeploymentXML::processJava()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != JavaTag) {
        return false;
    }

    QString javaVersion = "";
    QString javaFile = "";
    QString javaHash = "";
    QString javaOs = "";

    // TODO false si les attributs n'existent pas
    if (m_xmlReader.attributes().hasAttribute(JavaVersionAttribute)) {
        javaVersion = m_xmlReader.attributes().value(JavaVersionAttribute).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(HrefAttribute)) {
        javaFile = m_xmlReader.attributes().value(HrefAttribute).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(HashAttribute)) {
        javaHash = m_xmlReader.attributes().value(HashAttribute).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(OsAttribute)) {
        javaOs = m_xmlReader.attributes().value(OsAttribute).toString();
    }

    if (!javaVersion.isEmpty() && !javaFile.isEmpty() && !javaHash.isEmpty() && !javaOs.isEmpty()) {
        JavaUpdate ju(javaVersion, javaFile, javaHash);
        m_javaUpdates.insert(javaOs, ju);
    }

    return true;
} // DeploymentXML::processJava

bool DeploymentXML::processArguments()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != ArgumentsTag) {
        return false;
    }

    bool result = true;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == ArgumentTag) {
            result &= processArgument();
        }
        m_xmlReader.skipCurrentElement();
    }
    return result;
}

bool DeploymentXML::processArgument()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != ArgumentTag) {
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
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != DownloadsTag) {
        return false;
    }

    bool result = true;
    while (m_xmlReader.readNextStartElement()) {
        if (m_xmlReader.name() == ApplicationTag) {
            result &= processApplication();
        }
        m_xmlReader.skipCurrentElement();
    }
    return result;
}

bool DeploymentXML::processApplication()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != ApplicationTag) {
        return false;
    }

    Application * application = 0;

    QString name = "";
    QString version = "";
    QString downloaderExtensionClasspath = "";

    // TODO check attributes et return false
    if (m_xmlReader.attributes().hasAttribute(NameAttribute)) {
        name = m_xmlReader.attributes().value(NameAttribute).toString();
    }
    if (m_xmlReader.attributes().hasAttribute(VersionAttribute)) {
        version = m_xmlReader.attributes().value(VersionAttribute).toString();
    }
    if (m_xmlReader.attributes().hasAttribute(DownloaderExtensionClasspathAttribute)) {
        downloaderExtensionClasspath = m_xmlReader.attributes().value(DownloaderExtensionClasspathAttribute).toString();
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
            if ((m_xmlReader.name() == JarTag) || (m_xmlReader.name() == FileTag)) {
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
    QString os = OsAnyValue;
    bool native = false;
    bool main = false;

    if (m_xmlReader.attributes().hasAttribute(HrefAttribute)) {
        href = m_xmlReader.attributes().value(HrefAttribute).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(HashAttribute)) {
        hashMac = m_xmlReader.attributes().value(HashAttribute).toString();
    }

    if (m_xmlReader.attributes().hasAttribute(OsAttribute)) {
        QString osValue = m_xmlReader.attributes().value(OsAttribute).toString();
        if (osValue == OsWindowsValue) {
            os = OsWindowsValue;
        } else if (osValue == OsMacOsValue) {
            os = OsMacOsValue;
        }
    }

    if (m_xmlReader.attributes().hasAttribute(NativeAttribute) && m_xmlReader.attributes().value(NativeAttribute).toString() == "true") {
        native = true;
    }

    if (m_xmlReader.attributes().hasAttribute(MainAttribute) && m_xmlReader.attributes().value(MainAttribute).toString() == "true") {
        main = true;
    }

    return Download(href, hashMac, os, native, main);
} // DeploymentXML::processDownload
