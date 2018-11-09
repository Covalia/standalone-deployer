#include "commandline/commandlineparser.h"

#include <QCommandLineParser>
#include <QCommandLineOption>
#include "log/logger.h"
#include "settings/settings.h"
#include "io/config.h"

#include <QDir>

CommandLineParser::CommandLineParser()
{
    // silent mode
    QCommandLineOption silentOption("silent", "Enable silent mode. Install application without graphical interface.");

    // data location
    QCommandLineOption installLocationOption("installLocation", "Set the application installation path.", "installLocation");
    QCommandLineOption dataLocationOption("dataLocation", "Set the application data path.", "dataLocation");

    // proxy configuration
    QCommandLineOption proxyHostnameOption("proxyHostname", "Set the proxy hostname.", "proxyHostname");
    QCommandLineOption proxyPortOption("proxyPort", "Set the proxy port.", "proxyPort");
    QCommandLineOption proxyLoginOption("proxyLogin", "Set the proxy authentication login.", "proxyLogin");
    QCommandLineOption proxyPasswordOption("proxyPassword", "Set the proxy authentication password.", "proxyPassword");

    // language
    QCommandLineOption languageOption("language", "Set the application language. EN for English, FR for French.", "language");

    // start and shortcut
    QCommandLineOption runAppOption("runApp", "Enable the application launch after installation.");
    QCommandLineOption runAtStartOption("runAtStart", "Enable application launch when computer starts.");
    QCommandLineOption createOfflineShortcutOption("createOfflineShortcut", "Create offline shortcut.");
    QCommandLineOption createShortcutOption("createShortcut", "Create application shortcut.");
    QCommandLineOption createAllUserShortcutOption("createAllUserShortcut", "Create application shortcuts for all users. Option reserved when installation and data location are not in user folder.");

    QCommandLineParser parser;

    parser.setApplicationDescription("Covalia standalone deployer. Use --help option to display help.");
    parser.addHelpOption();
    parser.addVersionOption(); // TODO print version only, no logs.

    parser.addOption(silentOption);
    parser.addOption(installLocationOption);
    parser.addOption(dataLocationOption);
    parser.addOption(proxyHostnameOption);
    parser.addOption(proxyPortOption);
    parser.addOption(proxyLoginOption);
    parser.addOption(proxyPasswordOption);
    parser.addOption(languageOption);
    parser.addOption(runAppOption);
    parser.addOption(runAtStartOption);
    parser.addOption(createOfflineShortcutOption);
    parser.addOption(createShortcutOption);
    parser.addOption(createAllUserShortcutOption);

    L_INFO("Parsing command line arguments");

    L_INFO("Args: " + QCoreApplication::arguments().join(", "));

    parser.process(QCoreApplication::arguments());

    m_silent = parser.isSet(silentOption);
    m_installLocation = parser.value(installLocationOption);
    m_dataLocation = parser.value(dataLocationOption);
    m_proxyHostname = parser.value(proxyHostnameOption);
    m_proxyPort = parser.value(proxyPortOption);
    m_proxyLogin = parser.value(proxyLoginOption);
    m_proxyPassword = parser.value(proxyPasswordOption);
    m_language = parser.value(languageOption);
    m_runApp = parser.isSet(runAppOption);
    m_runAtStart = parser.isSet(runAtStartOption);
    m_createOfflineShortcut = parser.isSet(createOfflineShortcutOption);
    m_createShortcut = parser.isSet(createShortcutOption);
    m_createAllUserShortcut = parser.isSet(createAllUserShortcutOption);

    L_INFO("Parsed values");
    L_INFO("silent: " + QString(m_silent ? "yes" : "no"));
    L_INFO("installLocation: " + m_installLocation);
    L_INFO("dataLocation: " + m_dataLocation);
    L_INFO("proxyHostname: " + m_proxyHostname);
    L_INFO("proxyPort: " + m_proxyPort);
    L_INFO("proxyLogin: " + m_proxyLogin);
    L_INFO("language: " + m_language);
    L_INFO("runApp: " + QString(m_runApp ? "yes" : "no"));
    L_INFO("runAtStart: " + QString(m_runAtStart ? "yes" : "no"));
    L_INFO("createOfflineShortcut: " + QString(m_createOfflineShortcut ? "yes" : "no"));
    L_INFO("createShortcut: " + QString(m_createShortcut ? "yes" : "no"));
    L_INFO("createAllUserShortcut: " + QString(m_createAllUserShortcut ? "yes" : "no"));
}

void CommandLineParser::sendToSettings()
{
    Settings * settings = Settings::getInstance();

    if (!m_installLocation.isEmpty()) {
        settings->setDataLocation(m_installLocation + QDir::separator() + IOConfig::DataDir);
    }
    if (!m_dataLocation.isEmpty()) {
        settings->setDataLocation(m_dataLocation);
    }

    if (!m_proxyHostname.isEmpty() && !m_proxyPort.isEmpty()) {
        settings->setProxyUse(true);
    }

    if (!m_proxyHostname.isEmpty()) {
        settings->setProxyHostname(m_proxyHostname);
    }

    // port to int
    if (!m_proxyPort.isEmpty()) {
        bool okParsePort = false;
        int port = m_proxyPort.toInt(&okParsePort, 10);
        if (okParsePort) {
            if (port >= 0 && port <= 65535) {
                settings->setProxyPort(port);
            }
        }
    }

    if (!m_proxyLogin.isEmpty()) {
        settings->setProxyLogin(m_proxyLogin);
    }
    if (!m_proxyPassword.isEmpty()) {
        settings->setProxyPassword(m_proxyPassword);
    }

    settings->setShortcutOffline(m_createOfflineShortcut);
    settings->setShortcutOnline(m_createShortcut);
    settings->setShortcutForAllUsers(m_createAllUserShortcut);
    settings->setRunAtStart(m_runAtStart);
}

bool CommandLineParser::isRunApp() const
{
    return m_runApp;
}

void CommandLineParser::setRunApp(bool _runApp)
{
    m_runApp = _runApp;
}

bool CommandLineParser::isCreateAllUserShortcut() const
{
    return m_createAllUserShortcut;
}

void CommandLineParser::setCreateAllUserShortcut(bool _createAllUserShortcut)
{
    m_createAllUserShortcut = _createAllUserShortcut;
}

bool CommandLineParser::isCreateShortcut() const
{
    return m_createShortcut;
}

void CommandLineParser::setCreateShortcut(bool _createShortcut)
{
    m_createShortcut = _createShortcut;
}

bool CommandLineParser::isCreateOfflineShortcut() const
{
    return m_createOfflineShortcut;
}

void CommandLineParser::setOfflineShortcut(bool _createOfflineShortcut)
{
    m_createOfflineShortcut = _createOfflineShortcut;
}

bool CommandLineParser::isRunAtStart() const
{
    return m_runAtStart;
}

void CommandLineParser::setRunAtStart(bool _runAtStart)
{
    m_runAtStart = _runAtStart;
}

QString CommandLineParser::getLanguage() const
{
    return m_language;
}

void CommandLineParser::setLanguage(const QString &_language)
{
    m_language = _language;
}

QString CommandLineParser::getProxyPassword() const
{
    return m_proxyPassword;
}

void CommandLineParser::setProxyPassword(const QString &_proxyPassword)
{
    m_proxyPassword = _proxyPassword;
}

QString CommandLineParser::getProxyLogin() const
{
    return m_proxyLogin;
}

void CommandLineParser::setProxyLogin(const QString &_proxyLogin)
{
    m_proxyLogin = _proxyLogin;
}

QString CommandLineParser::getProxyPort() const
{
    return m_proxyPort;
}

void CommandLineParser::setProxyPort(const QString &_proxyPort)
{
    m_proxyPort = _proxyPort;
}

QString CommandLineParser::getProxyHostname() const
{
    return m_proxyHostname;
}

void CommandLineParser::setProxyHostname(const QString &_proxyHostname)
{
    m_proxyHostname = _proxyHostname;
}

QString CommandLineParser::getInstallLocation() const
{
    return m_installLocation;
}

void CommandLineParser::setInstallLocation(const QString &_installLocation)
{
    m_installLocation = _installLocation;
}

QString CommandLineParser::getDataLocation() const
{
    return m_dataLocation;
}

void CommandLineParser::setDataLocation(const QString &_dataLocation)
{
    m_dataLocation = _dataLocation;
}

bool CommandLineParser::isSilent() const
{
    return m_silent;
}

void CommandLineParser::setSilent(bool _silent)
{
    m_silent = _silent;
}
