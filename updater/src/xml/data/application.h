#ifndef UPDATER__APPLICATION_H
#define UPDATER__APPLICATION_H

#include <QString>
#include <QHash>
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
            return Application(UpdaterConfig::AppName, UpdaterConfig::AppCnlpRemoteFilename, UpdaterConfig::AppCnlpLocalFilename);
        }
        static Application getUpdaterApplication()
        {
            return Application(UpdaterConfig::UpdaterName, UpdaterConfig::UpdaterCnlpRemoteFilename, UpdaterConfig::UpdaterCnlpLocalFilename);
        }
        static Application getLoaderApplication()
        {
            return Application(UpdaterConfig::LoaderName, UpdaterConfig::LoaderCnlpRemoteFilename, UpdaterConfig::LoaderCnlpLocalFilename);
        }
        static Application getCnlpApplication()
        {
            return Application(UpdaterConfig::CnlpName, "", "");
        }
        static Application getEmptyApplication()
        {
            return Application("", "", "");
        }

        QString getName() const;
        QString getVersion() const;
        QString getCnlpRemoteFileName() const;
        QString getCnlpLocalFileName() const;
        QString getUpdaterExtensionClasspath() const;

        void setVersion(const QString &_version);
        void setUpdaterExtensionClasspath(const QString &_updaterExtensionClasspath);

        operator QString() const {
            return "Application [name=" + m_name +
                   ", version=" + m_version +
                   ", cnlpRemoteFileName=" + m_cnlpRemoteFileName +
                   ", cnlpLocalFileName=" + m_cnlpLocalFileName +
                   ", updaterExtensionClasspath=" + m_updaterExtensionClasspath + "]";
        }

    private:

        Application(QString _name,
                    QString _cnlpRemoteFileName,
                    QString _cnlpLocalFileName);

        QString m_name;
        QString m_version;
        QString m_cnlpRemoteFileName;
        QString m_cnlpLocalFileName;
        QString m_updaterExtensionClasspath;
};

inline bool operator<(const Application &_e1, const Application &_e2)
{
    return _e1.getName() < _e2.getName();
}

inline bool operator==(const Application &_e1, const Application &_e2)
{
    return _e1.getName() == _e2.getName()
           && _e1.getVersion() == _e2.getVersion()
           && _e1.getCnlpRemoteFileName() == _e2.getCnlpRemoteFileName()
           && _e1.getCnlpLocalFileName() == _e2.getCnlpLocalFileName()
           && _e1.getUpdaterExtensionClasspath() == _e2.getUpdaterExtensionClasspath();
}

inline uint qHash(const Application &_key, uint _seed)
{
    return qHash(_key.getName(), _seed)
           ^ qHash(_key.getVersion(), _seed)
           ^ qHash(_key.getCnlpRemoteFileName(), _seed)
           ^ qHash(_key.getCnlpLocalFileName(), _seed)
           ^ qHash(_key.getUpdaterExtensionClasspath(), _seed);
}

#endif // ifndef UPDATER__APPLICATION_H
