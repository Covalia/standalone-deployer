#include "xml/data/application.h"

Application::Application(QString _name,
                         QString _cnlpRemoteFileName,
                         QString _cnlpLocalFileName) :
    m_name(_name),
    m_version(""),
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
