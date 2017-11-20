#include "app/application.h"

Application::Application(QString _name,
                         QString _cnlpRemoteFileName,
                         QString _cnlpLocalFileName) :
    m_name(_name),
    m_cnlpRemoteFileName(_cnlpRemoteFileName),
    m_cnlpLocalFileName(_cnlpLocalFileName)
{
}

QString Application::getName() const
{
    return m_name;
}

QString Application::getVersion() const
{
    return m_version;
}

void Application::setVersion(const QString &_version)
{
    m_version = _version;
}

QString Application::getCnlpRemoteFileName() const
{
    return m_cnlpRemoteFileName;
}

QString Application::getCnlpLocalFileName() const
{
    return m_cnlpLocalFileName;
}

QString Application::getDownloaderExtensionClasspath() const
{
    return m_downloaderExtensionClasspath;
}

void Application::setDownloaderExtensionClasspath(const QString &_downloaderExtensionClasspath)
{
    m_downloaderExtensionClasspath = _downloaderExtensionClasspath;
}
