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
    runAtStartOption("runAtStart", "Enable application launch when computer starts.")
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
#ifdef Q_OS_WIN
    parser.addOption(runAtStartOption);
#endif

    L_INFO("Parsing command line arguments");

    L_INFO(QString("Args: %1").arg(QCoreApplication::arguments().join(", ")));
    parser.process(QCoreApplication::arguments());

    L_INFO("Parsed values");
    L_INFO(QString("silent: %1").arg(QString(parser.isSet(silentOption) ? "yes" : "no")));
    L_INFO(QString("installLocation: %1").arg(parser.value(installLocationOption)));
    L_INFO(QString("dataLocation: %1").arg(parser.value(dataLocationOption)));
    L_INFO(QString("useProxy:%1").arg(QString(parser.isSet(proxyUsedOption) ? "yes" : "no")));
    L_INFO(QString("proxyHostname: %1").arg(parser.value(proxyHostnameOption)));
    L_INFO(QString("proxyPort: %1").arg(parser.value(proxyPortOption)));
    L_INFO(QString("proxyLogin: %1").arg(parser.value(proxyLoginOption)));
    L_INFO(QString("locale: %1").arg(parser.value(localeOption)));
    L_INFO(QString("noRunApp: %1").arg(QString(parser.isSet(noRunAppOption) ? "yes" : "no")));
    L_INFO(QString("runAtStart: %1").arg(QString(parser.isSet(runAtStartOption) ? "yes" : "no")));
}

bool CommandLineParser::isRunApp() const
{
    return !parser.isSet(noRunAppOption);
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
