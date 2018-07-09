#ifndef UPDATER__APPLICATION_H
#define UPDATER__APPLICATION_H

#include <QString>
#include <QHash>
#include "io/config.h"
#include "updater/config.h"

/**
 * @class Application
 * \brief This class represents application informations in CNLP file. It contain application version, name etc
 */
class Application
{
    public:

        static Application getAppApplication()
        {
            return Application(IOConfig::AppName, UpdaterConfig::AppCnlpRemoteFilename, UpdaterConfig::AppCnlpLocalFilename);
        }
        static Application getUpdaterApplication()
        {
            return Application(IOConfig::UpdaterName, UpdaterConfig::UpdaterCnlpRemoteFilename, UpdaterConfig::UpdaterCnlpLocalFilename);
        }
        static Application getLoaderApplication()
        {
            return Application(IOConfig::LoaderName, UpdaterConfig::LoaderCnlpRemoteFilename, UpdaterConfig::LoaderCnlpLocalFilename);
        }
        static Application getCnlpApplication()
        {
            return Application(IOConfig::CnlpName, "", "");
        }
        static Application getJavaApplication()
        {
            return Application(IOConfig::JavaName, UpdaterConfig::JavaCnlpRemoteFilename, UpdaterConfig::JavaCnlpLocalFilename);
        }
        static Application getEmptyApplication()
        {
            return Application("", "", "");
        }

        QString getName() const;
        QString getVersion() const;
        QString getCnlpRemoteFileName() const;
        QString getCnlpLocalFileName() const;

        void setVersion(const QString &_version);

        operator QString() const {
            return "Application [name=" + m_name +
                   ", version=" + m_version +
                   ", cnlpRemoteFileName=" + m_cnlpRemoteFileName +
                   ", cnlpLocalFileName=" + m_cnlpLocalFileName + "]";
        }

    private:

        Application(QString _name,
                    QString _cnlpRemoteFileName,
                    QString _cnlpLocalFileName);

        QString m_name;
        QString m_version;
        QString m_cnlpRemoteFileName;
        QString m_cnlpLocalFileName;
};

inline bool operator<(const Application &_e1, const Application &_e2)
{
    if (_e1.getName() < _e2.getName()) {
        return true;
    } else {
        if (_e1.getVersion() < _e2.getVersion()) {
            return true;
        } else {
            if (_e1.getCnlpRemoteFileName() < _e2.getCnlpRemoteFileName()) {
                return true;
            }
            else {
                if (_e1.getCnlpLocalFileName() < _e2.getCnlpLocalFileName()) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
    }
}

inline bool operator==(const Application &_e1, const Application &_e2)
{
    return _e1.getName() == _e2.getName()
           && _e1.getVersion() == _e2.getVersion()
           && _e1.getCnlpRemoteFileName() == _e2.getCnlpRemoteFileName()
           && _e1.getCnlpLocalFileName() == _e2.getCnlpLocalFileName();
}

inline bool operator!=(const Application &_e1, const Application &_e2)
{
    return _e1.getName() != _e2.getName()
            || _e1.getVersion() != _e2.getVersion()
            || _e1.getCnlpRemoteFileName() != _e2.getCnlpRemoteFileName()
            || _e1.getCnlpLocalFileName() != _e2.getCnlpLocalFileName();
}

inline uint qHash(const Application &_key, uint _seed)
{
    return qHash(_key.getName(), _seed)
           ^ qHash(_key.getVersion(), _seed)
           ^ qHash(_key.getCnlpRemoteFileName(), _seed)
           ^ qHash(_key.getCnlpLocalFileName(), _seed);
}

#endif // ifndef UPDATER__APPLICATION_H
