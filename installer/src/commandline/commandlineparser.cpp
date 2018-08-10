#include "commandline/commandlineparser.h"

#include <QCommandLineParser>
#include <QCommandLineOption>
#include "log/logger.h"
#include "settings/settings.h"
#include "io/config.h"

#include <QDir>

const QString CommandLineParser::Empty("EMPTY");

CommandLineParser::CommandLineParser()
{
    // silent mode
    QCommandLineOption silentOption("silent", "Enable silent mode. Install application without graphical interface.");

    // data location
    QCommandLineOption installLocationOption("installLocation", "Set the application installation path.", "installLocation", Empty);
    QCommandLineOption dataLocationOption("dataLocation", "Set the application data path.", "dataLocation", Empty);

    // proxy configuration
    QCommandLineOption proxyHostnameOption("proxyHostname", "Set the proxy hostname.", "proxyHostname", Empty);
    QCommandLineOption proxyPortOption("proxyPort", "Set the proxy port.", "proxyPort", Empty);
    QCommandLineOption proxyLoginOption("proxyLogin", "Set the proxy authentication login.", "proxyLogin", Empty);
    QCommandLineOption proxyPasswordOption("proxyPassword", "Set the proxy authentication password.", "proxyPassword", Empty);

    // language
    QCommandLineOption languageOption("language", "Set the application language. EN for English, FR for French.", "language", Empty);

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
    m_installLocation = getValueString(parser, installLocationOption);
    m_dataLocation = getValueString(parser, dataLocationOption);
    m_proxyHostname = getValueString(parser, proxyHostnameOption);
    m_proxyPort = getValueString(parser, proxyPortOption);
    m_proxyLogin = getValueString(parser, proxyLoginOption);
    m_proxyPassword = getValueString(parser, proxyPasswordOption);
    m_language = getValueString(parser, languageOption);
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

    if (!isEmptyValue(m_installLocation)) {
        settings->setDataLocation(m_installLocation + QDir::separator() + IOConfig::DataDir);
    }
    if (!isEmptyValue(m_dataLocation)) {
        settings->setDataLocation(m_dataLocation);
    }

    if (!isEmptyValue(m_proxyHostname) && !isEmptyValue(m_proxyPort)) {
        settings->setProxyUse(true);
    }

    if (!isEmptyValue(m_proxyHostname)) {
        settings->setProxyHostname(m_proxyHostname);
    }

    // port to int
    if (!isEmptyValue(m_proxyPort)) {
        bool okParsePort = false;
        int port = m_proxyPort.toInt(&okParsePort, 10);
        if (okParsePort) {
            if (port >= 0 && port <= 65535) {
                settings->setProxyPort(port);
            }
        }
    }

    if (!isEmptyValue(m_proxyLogin)) {
        settings->setProxyLogin(m_proxyLogin);
    }
    if (!isEmptyValue(m_proxyPassword)) {
        settings->setProxyPassword(m_proxyPassword);
    }

    settings->setShortcutOffline(m_createOfflineShortcut);
    settings->setShortcutOnline(m_createShortcut);
    settings->setShortcutForAllUsers(m_createAllUserShortcut);
    settings->setRunAtStart(m_runAtStart);
}

QString CommandLineParser::getValueString(QCommandLineParser & parser, QCommandLineOption & commandOption)
{
    return parser.value(commandOption);
}

bool CommandLineParser::isEmptyValue(QString value)
{
    return value == Empty;
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

void CommandLineParser::setLanguage(const QString &language)
{
    m_language = language;
}

QString CommandLineParser::getProxyPassword() const
{
    return m_proxyPassword;
}

void CommandLineParser::setProxyPassword(const QString &proxyPassword)
{
    m_proxyPassword = proxyPassword;
}

QString CommandLineParser::getProxyLogin() const
{
    return m_proxyLogin;
}

void CommandLineParser::setProxyLogin(const QString &proxyLogin)
{
    m_proxyLogin = proxyLogin;
}

QString CommandLineParser::getProxyPort() const
{
    return m_proxyPort;
}

void CommandLineParser::setProxyPort(const QString &proxyPort)
{
    m_proxyPort = proxyPort;
}

QString CommandLineParser::getProxyHostname() const
{
    return m_proxyHostname;
}

void CommandLineParser::setProxyHostname(const QString &proxyHostname)
{
    m_proxyHostname = proxyHostname;
}

QString CommandLineParser::getDataLocation() const
{
    return m_dataLocation;
}

void CommandLineParser::setDataLocation(const QString &dataLocation)
{
    m_dataLocation = dataLocation;
}

bool CommandLineParser::isSilent() const
{
    return m_silent;
}

void CommandLineParser::setSilent(bool _silent)
{
    m_silent = _silent;
}
