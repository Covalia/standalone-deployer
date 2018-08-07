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
    // silence mode
    QCommandLineOption silent_opt("silent", "Silent mode : no configuration interface [--silent]", "silent", Empty);

    // data location
    QCommandLineOption installLocation_opt("installLocation", "Application installation path [--installLocation=\"XXX\"].", "installLocation", Empty);
    QCommandLineOption dataLocation_opt("dataLocation", "Application data path [--dataLocation=\"XXX\"]", "dataLocation", Empty);

    // proxy configuration
    QCommandLineOption proxyAuto_opt("proxyAuto", "Automatic proxy detection. Use this option conbined with proxyUse [--proxyUse=true --proxyAuto=true]", "proxyAuto", Empty);
    QCommandLineOption proxyHostname_opt("proxyHostname", "Proxy Hostname [--proxyHostname=XXXX]", "proxyHostname", Empty);
    QCommandLineOption proxyPort_opt("proxyPort", "Proxy Port [--proxyPort=XXXX]", "proxyPort", Empty);
    QCommandLineOption proxyLogin_opt("proxyLogin", "Use login in proxy authentification [--proxyLogin=XXX]", "proxyLogin", Empty);
    QCommandLineOption proxyPassword_opt("proxyPassword", "Use password in proxy authentification [--proxyPassword=XXX]", "proxyPassword", Empty);

    // language
    QCommandLineOption language_opt("language", "Language. EN=English, FR=French; [--language=FR]", "language", Empty);

    // start and shortcut
    QCommandLineOption runApp_opt("runApp", "Launch application after installation. [--runApp=true]", "runApp", Empty);
    QCommandLineOption runAtStart_opt("runAtStart", "Launch application when computer starts. [--runAtStart=true]", "runAtStart", Empty);
    QCommandLineOption createOfflineShortcut_opt("createOfflineShortcut", "Create offline shortcut. [--createOfflineShortcut=true]", "createOfflineShortcut", Empty);
    QCommandLineOption createShortcut_opt("createShortcut", "Create application shortcut. [--shortcurt=false]", "createShortcut", Empty);
    QCommandLineOption createAllUserShortcut_opt("createAllUserShortcut", "Create application shortcuts for all users. Option reserved when installation and data location are not in user folder [--createAllUserShortcut=false]", "createAllUserShortcut", Empty);

    QCommandLineParser parser;

    parser.setApplicationDescription("Standalone deployment. Set --help option to display help");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption(silent_opt);
    parser.addOption(installLocation_opt);
    parser.addOption(dataLocation_opt);
    parser.addOption(proxyAuto_opt);
    parser.addOption(proxyHostname_opt);
    parser.addOption(proxyPort_opt);
    parser.addOption(proxyLogin_opt);
    parser.addOption(proxyPassword_opt);
    parser.addOption(language_opt);
    parser.addOption(runApp_opt);
    parser.addOption(runAtStart_opt);
    parser.addOption(createOfflineShortcut_opt);
    parser.addOption(createShortcut_opt);
    parser.addOption(createAllUserShortcut_opt);

    L_INFO("Start to parse command line arguments");

    parser.process(QCoreApplication::arguments());

    L_INFO("End to parse command line arguments");
    L_INFO("Start to read command line arguments with parse data");

    m_silent = getValueBool(parser, silent_opt);
    m_installLocation = getValueString(parser, installLocation_opt);
    m_dataLocation = getValueString(parser, dataLocation_opt);
    m_proxyAuto = getValueBool(parser, proxyAuto_opt);
    m_proxyHostname = getValueString(parser, proxyHostname_opt);
    m_proxyPort = getValueString(parser, proxyPort_opt);
    m_proxyLogin = getValueString(parser, proxyLogin_opt);
    m_proxyPassword = getValueString(parser, proxyPassword_opt);
    m_language = getValueString(parser, language_opt);
    m_runApp = getValueBool(parser, runApp_opt);
    m_runAtStart = getValueBool(parser, runAtStart_opt);
    m_offlineShortcut = getValueBool(parser, createOfflineShortcut_opt);
    m_shortcut = getValueBool(parser, createShortcut_opt);
    m_allUserShortcut = getValueBool(parser, createAllUserShortcut_opt);

    L_INFO("End to read command line arguments with parse data");
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

    if (!isEmptyValue(m_proxyAuto)) {
        settings->setProxyAuto(parseBool(m_proxyAuto));
        // activate proxy automatically
        if (parseBool(m_proxyAuto)) {
            settings->setProxyUse(true);
        }
    }
    if (!isEmptyValue(m_proxyHostname)) {
        settings->setProxyUse(true);
        settings->setProxyHostname(m_proxyHostname);
        settings->setProxyAuto(false);
        settings->setProxyManual(true);
    }

    // port to int
    if (!isEmptyValue(m_proxyPort)) {
        bool okParsePort = false;
        int port = m_proxyPort.toInt(&okParsePort, 10);
        if (okParsePort) {
            settings->setProxyPort(port);
        }
    }

    if (!isEmptyValue(m_proxyLogin)) {
        settings->setProxyLogin(m_proxyLogin);
        settings->setProxyAuthentification(true);
    }
    if (!isEmptyValue(m_proxyPassword)) {
        settings->setProxyPassword(m_proxyPassword);
    }

    if (!isEmptyValue(m_offlineShortcut)) {
        settings->setShortcutOffline(parseBool(m_offlineShortcut));
    }
    if (!isEmptyValue(m_shortcut)) {
        settings->setShortcutOnline(parseBool(m_shortcut));
    }
    if (!isEmptyValue(m_allUserShortcut)) {
        settings->setShortcutForAllUsers(parseBool(m_allUserShortcut));
    }

    if (!isEmptyValue(m_runAtStart)) {
        settings->setRunAtStart(parseBool(m_runAtStart));
    }
} // CommandLineParser::sendToSettings

QString CommandLineParser::getValueBool(QCommandLineParser & parser, QCommandLineOption & commandOption)
{
    if (parser.isSet(commandOption)) {
        if (parser.value(commandOption) == "false") {
            return "false";
        } else {
            return "true";
        }
    } else {
        return Empty;
    }
}

QString CommandLineParser::getValueString(QCommandLineParser & parser, QCommandLineOption & commandOption)
{
    return parser.value(commandOption);
}

bool CommandLineParser::isEmptyValue(QString value)
{
    return value == Empty;
}

bool CommandLineParser::parseBool(QString value)
{
    return value == "true";
}

bool CommandLineParser::parseBoolWithDefaultValue(QString value, bool defaultValue)
{
    if (isEmptyValue(value)) {
        return defaultValue;
    }
    return parseBool(value);
}

bool CommandLineParser::isSilent()
{
    return parseBoolWithDefaultValue(m_silent, false);
}

bool CommandLineParser::isRunApp()
{
    return parseBoolWithDefaultValue(m_runApp, true);
}

QString CommandLineParser::getAllUserShortcut() const
{
    return m_allUserShortcut;
}

QString CommandLineParser::getRunApp() const
{
    return m_runApp;
}

void CommandLineParser::setRunApp(const QString &runApp)
{
    m_runApp = runApp;
}

void CommandLineParser::setAllUserShortcut(const QString &allUserShortcut)
{
    m_allUserShortcut = allUserShortcut;
}

QString CommandLineParser::getShortcut() const
{
    return m_shortcut;
}

void CommandLineParser::setShortcut(const QString &shortcut)
{
    m_shortcut = shortcut;
}

QString CommandLineParser::getOffshort() const
{
    return m_offlineShortcut;
}

void CommandLineParser::setOffshort(const QString &offshort)
{
    m_offlineShortcut = offshort;
}

QString CommandLineParser::getRunAtStart() const
{
    return m_runAtStart;
}

void CommandLineParser::setRunAtStart(const QString &runAtStart)
{
    m_runAtStart = runAtStart;
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

QString CommandLineParser::getProxyAuto() const
{
    return m_proxyAuto;
}

void CommandLineParser::setProxyAuto(const QString &proxyAuto)
{
    m_proxyAuto = proxyAuto;
}

QString CommandLineParser::getDataLocation() const
{
    return m_dataLocation;
}

void CommandLineParser::setDataLocation(const QString &dataLocation)
{
    m_dataLocation = dataLocation;
}

QString CommandLineParser::getSilent() const
{
    return m_silent;
}

void CommandLineParser::setSilent(const QString &silent)
{
    m_silent = silent;
}
