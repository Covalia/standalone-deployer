#ifndef INSTALLER__COMMANDLINEPARSER_H
#define INSTALLER__COMMANDLINEPARSER_H

#include <QCommandLineParser>
#include <QCommandLineOption>

/**
 * @class CommandLineParser
 *
 * @brief Read the launch the command line arguments and save them in this class.
 */
class CommandLineParser
{
    public:

        CommandLineParser();

        bool isSilent() const;

        QString getInstallLocation() const;
        QString getDataLocation() const;

        bool isProxyUsed() const;
        bool isProxyHostnameSet() const;
        QString getProxyHostname() const;
        bool isProxyPortSet() const;
        QString getProxyPort() const;
        bool isProxyLoginSet() const;
        QString getProxyLogin() const;
        bool isProxyPasswordSet() const;
        QString getProxyPassword() const;

        bool isLocaleSet() const;
        QString getLocale() const;
        void setLocale(const QString &_locale);

        bool isRunApp() const;
        bool isRunAtStart() const;
        bool isCreateOfflineShortcut() const;
        bool isCreateAllUserShortcut() const;

    private:

        QCommandLineParser parser;

        // silent mode
        QCommandLineOption silentOption;

        // data location
        QCommandLineOption installLocationOption;
        QCommandLineOption dataLocationOption;

        // proxy configuration
        QCommandLineOption proxyUsedOption;
        QCommandLineOption proxyHostnameOption;
        QCommandLineOption proxyPortOption;
        QCommandLineOption proxyLoginOption;
        QCommandLineOption proxyPasswordOption;

        // locale
        QCommandLineOption localeOption;

        // start and shortcut
        QCommandLineOption noRunAppOption;
        QCommandLineOption runAtStartOption;
        QCommandLineOption createOfflineShortcutOption;
        QCommandLineOption createAllUserShortcutOption;
};

#endif
