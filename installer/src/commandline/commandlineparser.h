#ifndef INSTALLER__COMMANDLINEPARSER_H
#define INSTALLER__COMMANDLINEPARSER_H

#include <QCommandLineParser>

/**
 * @class QCommandLineParser
 *
 * @brief Read the launch the command line arguments and save them in this class.
 */
class CommandLineParser
{
    public:

        CommandLineParser();

        void sendToSettings();

        bool isSilent() const;
        void setSilent(bool _silent);

        QString getInstallLocation() const;
        void setInstallLocation(const QString &_installLocation);

        QString getDataLocation() const;
        void setDataLocation(const QString &_dataLocation);

        QString getProxyHostname() const;
        void setProxyHostname(const QString &_proxyHostname);

        QString getProxyPort() const;
        void setProxyPort(const QString &_proxyPort);

        QString getProxyLogin() const;
        void setProxyLogin(const QString &_proxyLogin);

        QString getProxyPassword() const;
        void setProxyPassword(const QString &_proxyPassword);

        QString getLanguage() const;
        void setLanguage(const QString &_language);

        bool isRunApp() const;
        void setRunApp(bool _runApp);

        bool isRunAtStart() const;
        void setRunAtStart(bool _runAtStart);

        bool isCreateOfflineShortcut() const;
        void setOfflineShortcut(bool _createOfflineShortcut);

        bool isCreateShortcut() const;
        void setCreateShortcut(bool _createShortcut);

        bool isCreateAllUserShortcut() const;
        void setCreateAllUserShortcut(bool _createAllUserShortcut);

    private:

        bool m_silent;
        QString m_installLocation;
        QString m_dataLocation;
        QString m_proxyHostname;
        QString m_proxyPort;
        QString m_proxyLogin;
        QString m_proxyPassword;
        QString m_language;
        bool m_runApp;
        bool m_runAtStart;
        bool m_createOfflineShortcut;
        bool m_createShortcut;
        bool m_createAllUserShortcut;
};

#endif
