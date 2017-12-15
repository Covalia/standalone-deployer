#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

#include <QCommandLineParser>


const QString EMPTY = "EMPTY";

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

        QString getSilent() const;
        void setSilent(const QString &silent);

        QString getIntallLocation() const;
        void setIntallLocation(const QString &intallLocation);

        QString getDataLocation() const;
        void setDataLocation(const QString &dataLocation);

        QString getProxyAuto() const;
        void setProxyAuto(const QString &proxyAuto);

        QString getProxyHostname() const;
        void setProxyHostname(const QString &proxyHostname);

        QString getProxyPort() const;
        void setProxyPort(const QString &proxyPort);

        QString getProxyLogin() const;
        void setProxyLogin(const QString &proxyLogin);

        QString getProxyPassword() const;
        void setProxyPassword(const QString &proxyPassword);

        QString getLanguage() const;
        void setLanguage(const QString &language);

        QString getRunAtStart() const;
        void setRunAtStart(const QString &runAtStart);

        QString getOffshort() const;
        void setOffshort(const QString &offshort);

        QString getShortcut() const;
        void setShortcut(const QString &shortcut);

        QString getAllUserShortcut() const;
        void setAllUserShortcut(const QString &allUserShortcut);

private:

        /**
         * @brief Method used to read boolean arguments
         * @param parser
         * @param commandOption
         * @return true if argument exist and have no value or true value. Example : --arg OR--arg=true
         */
        QString getValueBool(QCommandLineParser & parser, QCommandLineOption & commandOption);

        /**
         * @brief Method used to read string arguments
         * @param parser
         * @param commandOption
         * @return value unless the value is "EMPTY" (defaut arg value) return empty value ("").
         */
        QString getValueString(QCommandLineParser & parser, QCommandLineOption & commandOption);

        /**
         * @brief Return true is value is empty : not definid in arg of parser line
         * @param value
         * @return
         */
        bool isEmptyValue(QString value);

        /**
         * @brief Parse bool value
         * @param value
         * @return
         */
        bool parseBool(QString value);

        QString m_silent;
        QString m_intallLocation;
        QString m_dataLocation;
        QString m_proxyAuto;
        QString m_proxyHostname;
        QString m_proxyPort;
        QString m_proxyLogin;
        QString m_proxyPassword;
        QString m_language;
        QString m_runAtStart;
        QString m_offshort;
        QString m_shortcut;
        QString m_allUserShortcut;
};

#endif // COMMANDLINEPARSER_H
