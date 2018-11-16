#include "commandline/commandlineparser.h"

#include "log/logger.h"
#include "io/config.h"

CommandLineParser::CommandLineParser() :
    parser(),
    silentOption("silent", "Enable silent mode. Install application without graphical interface."),
    installLocationOption("installLocation", "Set the application installation path.", "installLocation"),
    dataLocationOption("dataLocation", "Set the application data path.", "dataLocation"),
    proxyUsedOption("useProxy", "Enable the use of proxy."),
    proxyHostnameOption("proxyHostname", "Set the proxy hostname.", "proxyHostname"),
    proxyPortOption("proxyPort", "Set the proxy port.", "proxyPort"),
    proxyLoginOption("proxyLogin", "Set the proxy authentication login.", "proxyLogin"),
    proxyPasswordOption("proxyPassword", "Set the proxy authentication password.", "proxyPassword"),
    localeOption("locale", QString("Set the application locale. %1 for English, %2 for French.").arg(IOConfig::LocaleEnUs).arg(IOConfig::LocaleFrFr), "locale"),
    noRunAppOption("noRunApp", "Does not start the application after installation."),
    runAtStartOption("runAtStart", "Enable application launch when computer starts."),
    createOfflineShortcutOption("createOfflineShortcut", "Create offline shortcut."),
    createAllUserShortcutOption("createAllUserShortcut", "Not yes implemented!!! Create application shortcuts for all users. Option reserved when installation and data location are not in user folder.")
{
    parser.setApplicationDescription("Covalia standalone deployer. Use --help option to display help.");
    parser.addHelpOption();
    parser.addVersionOption(); // TODO print version only, no logs.

    parser.addOption(silentOption);
    parser.addOption(installLocationOption);
    parser.addOption(dataLocationOption);
    parser.addOption(proxyUsedOption);
    parser.addOption(proxyHostnameOption);
    parser.addOption(proxyPortOption);
    parser.addOption(proxyLoginOption);
    parser.addOption(proxyPasswordOption);
    parser.addOption(localeOption);
    parser.addOption(noRunAppOption);
    parser.addOption(runAtStartOption);
    parser.addOption(createOfflineShortcutOption);
    parser.addOption(createAllUserShortcutOption);

    L_INFO("Parsing command line arguments");

    L_INFO("Args: " + QCoreApplication::arguments().join(", "));
    parser.process(QCoreApplication::arguments());

    L_INFO("Parsed values");
    L_INFO("silent: " + QString(parser.isSet(silentOption) ? "yes" : "no"));
    L_INFO("installLocation: " + parser.value(installLocationOption));
    L_INFO("dataLocation: " + parser.value(dataLocationOption));
    L_INFO("useProxy:" + QString(parser.isSet(proxyUsedOption) ? "yes" : "no"));
    L_INFO("proxyHostname: " + parser.value(proxyHostnameOption));
    L_INFO("proxyPort: " + parser.value(proxyPortOption));
    L_INFO("proxyLogin: " + parser.value(proxyLoginOption));
    L_INFO("locale: " + parser.value(localeOption));
    L_INFO("noRunApp: " + QString(parser.isSet(noRunAppOption) ? "yes" : "no"));
    L_INFO("runAtStart: " + QString(parser.isSet(runAtStartOption) ? "yes" : "no"));
    L_INFO("createOfflineShortcut: " + QString(parser.isSet(createOfflineShortcutOption) ? "yes" : "no"));
    L_INFO("createAllUserShortcut: " + QString(parser.isSet(createAllUserShortcutOption) ? "yes" : "no"));
}

bool CommandLineParser::isRunApp() const
{
    return !parser.isSet(noRunAppOption);
}

bool CommandLineParser::isCreateAllUserShortcut() const
{
    return parser.isSet(createAllUserShortcutOption);
}

bool CommandLineParser::isCreateOfflineShortcut() const
{
    return parser.isSet(createOfflineShortcutOption);
}

bool CommandLineParser::isRunAtStart() const
{
    return parser.isSet(runAtStartOption);
}

bool CommandLineParser::isProxyPasswordSet() const
{
    return parser.isSet(proxyPasswordOption);
}

bool CommandLineParser::isLocaleSet() const
{
    return parser.isSet(localeOption);
}

QString CommandLineParser::getLocale() const
{
    return parser.value(localeOption);
}

QString CommandLineParser::getProxyPassword() const
{
    return parser.value(proxyPasswordOption);
}

bool CommandLineParser::isProxyLoginSet() const
{
    return parser.isSet(proxyLoginOption);
}

QString CommandLineParser::getProxyLogin() const
{
    return parser.value(proxyLoginOption);
}

bool CommandLineParser::isProxyPortSet() const
{
    return parser.isSet(proxyPortOption);
}

QString CommandLineParser::getProxyPort() const
{
    return parser.value(proxyPortOption);
}

bool CommandLineParser::isProxyHostnameSet() const
{
    return parser.isSet(proxyHostnameOption);
}

QString CommandLineParser::getProxyHostname() const
{
    return parser.value(proxyHostnameOption);
}

QString CommandLineParser::getInstallLocation() const
{
    return parser.value(installLocationOption);
}

QString CommandLineParser::getDataLocation() const
{
    return parser.value(dataLocationOption);
}

bool CommandLineParser::isSilent() const
{
    return parser.isSet(silentOption);
}

bool CommandLineParser::isProxyUsed() const
{
    return parser.isSet(proxyUsedOption);
}
