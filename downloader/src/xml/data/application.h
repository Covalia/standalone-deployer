#ifndef DOWNLOADER__APPLICATION_H
#define DOWNLOADER__APPLICATION_H

#include <QString>

/**
 * @class Application
 * \brief This class represents application informations in CNLP file. It contain application version, name etc
 */
class Application
{
    public:
        Application(QString _name,
                    QString _cnlpRemoteFileName,
                    QString _cnlpLocalFileName);

        QString getName() const;
        QString getVersion() const;
        QString getCnlpRemoteFileName() const;
        QString getCnlpLocalFileName() const;
        QString getDownloaderExtensionClasspath() const;

        void setVersion(const QString &_version);
        void setDownloaderExtensionClasspath(const QString &_downloaderExtensionClasspath);

        operator QString() const {
            return "Application [name=" + m_name +
                   ", version=" + m_version +
                   ", cnlpRemoteFileName=" + m_cnlpRemoteFileName +
                   ", cnlpLocalFileName=" + m_cnlpLocalFileName +
                   ", downloaderExtensionClasspath=" + m_downloaderExtensionClasspath + "]";
        }

    private:
        QString m_name;
        QString m_version;
        QString m_cnlpRemoteFileName;
        QString m_cnlpLocalFileName;
        QString m_downloaderExtensionClasspath;
};

inline bool operator<(const Application &_e1, const Application &_e2)
{
    return _e1.getName() < _e2.getName();
}

#endif // ifndef DOWNLOADER__APPLICATION_H
