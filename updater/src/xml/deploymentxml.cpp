#include "xml/deploymentxml.h"
#include "updater/config.h"
#include "io/config.h"
#include "log/logger.h"
#include <QFile>

const QString DeploymentXML::DeploymentTag("deployment");
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
const QString DeploymentXML::ClasspathAttribute("classpath");

const QString DeploymentXML::ApplicationTag("application");
const QString DeploymentXML::FileTag("file");

const QString DeploymentXML::VersionAttribute("version");
const QString DeploymentXML::NameAttribute("name");

const QString DeploymentXML::ArgumentsTag("arguments");
const QString DeploymentXML::ArgumentTag("argument");

const QString DeploymentXML::MemoryTag("memory");
const QString DeploymentXML::VersionTag("version");
const QString DeploymentXML::EncodingTag("encoding");
const QString DeploymentXML::MainClassTag("mainclass");
const QString DeploymentXML::RunnerClassTag("runnerclass");
const QString DeploymentXML::ServerNameTag("server_name");

DeploymentXML::DeploymentXML(const QString &_pathCnlp, QObject * _parent) :
    QObject(_parent),
    m_xmlFile(_pathCnlp),
    m_application(Application::getEmptyApplication())
{
    m_memory = "512";
    m_version = "";
    m_encoding = "";
    m_mainClass = "";
    m_runnerClass = "";
    m_serverName = "";
    m_downloads = QList<Download>();
    m_arguments = QList<QString>();
}

DeploymentXML::~DeploymentXML()
{
}

bool DeploymentXML::read()
{
    if (!m_xmlFile.open(QFile::ReadOnly | QFile::Text)) {
        L_ERROR(QString("Cannot read file: %1").arg(m_xmlFile.errorString()));
        return false;
    }

    m_xmlReader.setDevice(&m_xmlFile);

    // TODO verify xml error handling
    bool result = processDeployment();
    if (!result) {
        L_ERROR("Error processing deployment file.");
    }
    m_xmlFile.close();
    L_INFO("Closed deployment file.");

    return result;
}

QString DeploymentXML::readNextText()
{
    m_xmlReader.readNext();
    return m_xmlReader.text().toString();
}

Application DeploymentXML::getApplication() const
{
    return m_application;
}

QList<Download> DeploymentXML::getDownloads() const
{
    QList<Download> downloads;

    QString osValue;

#ifdef Q_OS_MACOS
        osValue = OsMacOsValue;
#endif

#ifdef Q_OS_WIN
        osValue = OsWindowsValue;
#endif

    QListIterator<Download> iterator(m_downloads);
    while (iterator.hasNext()) {
        const Download & download = iterator.next();
        if (download.getOs() == osValue || download.getOs() == OsAnyValue) {
            downloads.append(download);
        }
    }

    return downloads;
}

QString DeploymentXML::getMemory() const
{
    return m_memory;
}

QString DeploymentXML::getEncoding() const
{
    return m_encoding;
}

QString DeploymentXML::getMainClass() const
{
    return m_mainClass;
}

QString DeploymentXML::getRunnerClass() const
{
    return m_runnerClass;
}

QString DeploymentXML::getServerName() const
{
    return m_serverName;
}

QString DeploymentXML::getCurrentOsValue()
{
#ifdef Q_OS_MACOS
        return OsMacOsValue;
#endif

#ifdef Q_OS_WIN
        return OsWindowsValue;
#endif
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

    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != DeploymentTag) {
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
        } else if (m_xmlReader.name() == EncodingTag) {
            result &= processEncoding();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == MainClassTag) {
            result &= processMainClass();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == RunnerClassTag) {
            result &= processRunnerClass();
            m_xmlReader.skipCurrentElement();
        } else if (m_xmlReader.name() == ServerNameTag) {
            result &= processServerName();
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
}

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

bool DeploymentXML::processEncoding()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != EncodingTag) {
        return false;
    }
    m_encoding = readNextText();
    return true;
}

bool DeploymentXML::processMainClass()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != MainClassTag) {
        return false;
    }
    m_mainClass = readNextText();
    return true;
}

bool DeploymentXML::processRunnerClass()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != RunnerClassTag) {
        return false;
    }
    m_runnerClass = readNextText();
    return true;
}

bool DeploymentXML::processServerName()
{
    if (!m_xmlReader.isStartElement() || m_xmlReader.name() != ServerNameTag) {
        return false;
    }
    m_serverName = readNextText();
    return true;
}

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

    Application application = Application::getEmptyApplication();

    QString name = "";
    QString version = "";

    // TODO check attributes and return false
    if (m_xmlReader.attributes().hasAttribute(NameAttribute)) {
        name = m_xmlReader.attributes().value(NameAttribute).toString();
    }
    if (m_xmlReader.attributes().hasAttribute(VersionAttribute)) {
        version = m_xmlReader.attributes().value(VersionAttribute).toString();
    }

    if (name == IOConfig::AppName) {
        application = Application::getAppApplication();
    } else if (name == IOConfig::LoaderName) {
        application = Application::getLoaderApplication();
    } else if (name == IOConfig::UpdaterName) {
        application = Application::getUpdaterApplication();
    } else if (name == IOConfig::JavaName) {
        application = Application::getJavaApplication();
    }

    bool result = false;

    if (name == IOConfig::AppName || name == IOConfig::LoaderName || name == IOConfig::UpdaterName || name == IOConfig::JavaName) {
        application.setVersion(version);

        m_downloads.clear();
        m_application = application;

        while (m_xmlReader.readNextStartElement()) {
            if ((m_xmlReader.name() == JarTag) || (m_xmlReader.name() == FileTag)) {
                Download download = processDownload();
                m_downloads << download;
            }
            m_xmlReader.skipCurrentElement();
        }

        result = true;
    }

    return result;
}

Download DeploymentXML::processDownload()
{
    QString href = "";
    QString hashMac = "";
    QString os = OsAnyValue;
    bool native = false;
    bool inClasspath = false;
    QString version = "";

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

    if (m_xmlReader.attributes().hasAttribute(ClasspathAttribute) && m_xmlReader.attributes().value(ClasspathAttribute).toString() == "true") {
        inClasspath = true;
    }

    if (m_xmlReader.attributes().hasAttribute(VersionAttribute)) {
        version = m_xmlReader.attributes().value(VersionAttribute).toString();
    }

    return Download(href, hashMac, os, version, native, inClasspath);
}
