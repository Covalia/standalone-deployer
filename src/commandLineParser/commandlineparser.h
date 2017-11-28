#ifndef COMMANDLINEPARSER_H
#define COMMANDLINEPARSER_H

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

    bool getSilent() const;
    void setSilent(bool value);

    QString getIntallLocation() const;
    void setIntallLocation(const QString &value);

    QString getDataLocation() const;
    void setDataLocation(const QString &value);

    bool getProxyUse() const;
    void setProxyUse(bool value);

    bool getProxyAuto() const;
    void setProxyAuto(bool value);

    QString getProxyURL() const;
    void setProxyURL(const QString &value);

    QString getProxyPort() const;
    void setProxyPort(const QString &value);

    QString getProxyLogin() const;
    void setProxyLogin(const QString &value);

    QString getProxyPassword() const;
    void setProxyPassword(const QString &value);

    QString getLanguage() const;
    void setLanguage(const QString &value);

    bool getRunAtStart() const;
    void setRunAtStart(bool value);

    bool getOffshort() const;
    void setOffshort(bool value);

    bool getShortcut() const;
    void setShortcut(bool value);

    bool getAllUserShortcut() const;
    void setAllUserShortcut(bool value);

private:

    /**
     * @brief Method used to read boolean arguments
     * @param parser
     * @param commandOption
     * @return true if argument exist and have no value or true value. Example : --arg OR--arg=true
     */
    bool isExistingBooleanArg(QCommandLineParser & parser, QCommandLineOption & commandOption);

    /**
     * @brief Method used to read string arguments
     * @param parser
     * @param commandOption
     * @return value unless the value is "EMPTY" (defaut arg value) return empty value ("").
     */
    QString getValueArg(QCommandLineParser & parser, QCommandLineOption & commandOption);

    bool silent;
    QString intallLocation;
    QString dataLocation;
    bool proxyUse;
    bool proxyAuto;
    QString proxyURL;
    QString proxyPort;
    QString proxyLogin;
    QString proxyPassword;
    QString language;
    bool runAtStart;
    bool offshort;
    bool shortcut;
    bool allUserShortcut;

};

#endif // COMMANDLINEPARSER_H
