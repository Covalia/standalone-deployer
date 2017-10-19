#include "application.h"


Application::Application(QString _name,
                         QString _cnlpRemoteFileName,
                         QString _cnlpLocalFileName)
{
    m_name = _name;
    m_cnlpRemoteFileName = _cnlpRemoteFileName;
    m_cnlpLocalFileName = _cnlpLocalFileName;
}

QString Application::getName() const
{
    return m_name;
}

void Application::setName(const QString &name)
{
    m_name = name;
}

QString Application::getVersion() const
{
    return m_version;
}

void Application::setVersion(const QString &version)
{
    m_version = version;
}

QString Application::getCnlpRemoteFileName() const
{
    return m_cnlpRemoteFileName;
}

void Application::setCnlpRemoteFileName(const QString &cnlpRemoteFileName)
{
    m_cnlpRemoteFileName = cnlpRemoteFileName;
}

QString Application::getCnlpLocalFileName() const
{
    return m_cnlpLocalFileName;
}

void Application::setCnlpLocalFileName(const QString &cnlpLocalFileName)
{
    m_cnlpLocalFileName = cnlpLocalFileName;
}

QString Application::getDownloaderExtensionClasspath() const
{
    return m_downloaderExtensionClasspath;
}

void Application::setDownloaderExtensionClasspath(const QString &downloaderExtensionClasspath)
{
    m_downloaderExtensionClasspath = downloaderExtensionClasspath;
}



