#include "commandlineparser.h"

#include <QCommandLineParser>
#include <QCommandLineOption>
#include "../log/simpleqtlogger.h"

CommandLineParser::CommandLineParser(){
    // silence mode
    QCommandLineOption silent_opt({ "s", "silent" }, "Silent mode : no configuration interface [--silent]", "false");

    // data location
    QCommandLineOption intallLocation_opt({ "i", "intallLocation" }, "Application installation path [intallLocation=\"XXX\"]", "EMPTY");
    QCommandLineOption dataLocation_opt({ "d", "dataLocation" }, "Application data path. Dafault value is \"installation path/Data\". [dataLocation=\"XXX\"]", "EMPTY");

    // proxy configuration
    QCommandLineOption proxyUse_opt({ "p", "proxyUse" }, "Use if proxy is configured [--proxyUse]", "false");
    QCommandLineOption proxyAuto_opt({ "pauto", "proxyAuto" }, "Automatic proxy detection. Use this option conbined with proxyUse [--proxyUse --proxyAuto]", "false");
    QCommandLineOption proxyURL_opt({ "purl", "proxyURL" }, "Proxy URL [--proxyURL=XXXX]",  "EMPTY");
    QCommandLineOption proxyPort_opt({ "pport", "proxyPort" }, "Proxy Port [--proxyPort=XXXX]",  "EMPTY");
    QCommandLineOption proxyLogin_opt({ "plogin", "proxyLogin" }, "Use login in proxy authentification [--proxyLogin=XXX]",  "EMPTY");
    QCommandLineOption proxyPassword_opt({ "ppwd", "proxyPassword" }, "Use password in proxy authentification [--proxyPassword=XXX]",  "EMPTY");

    // language
    QCommandLineOption language_opt({ "l", "language" }, "Language. EN=English,FR=French; [language=FR]",  "EMPTY");

    // start and shortcut
    QCommandLineOption runAtStart_opt({ "ras", "runAtStart" }, "Launch application when computer starts. Default value is false [runAtStart=true]", "false");
    QCommandLineOption createOfflineShortcut_opt({ "offSh", "createOfflineShortcut" }, "Create offline shortcut. Dafault value is false [createOfflineShortcut=true]", "false");
    QCommandLineOption createShortcut_opt({ "sh", "createShortcut" }, "Create application shortcut. Default value is true [shortcurt=false]", "true");
    QCommandLineOption createAllUserShortcut_opt({ "allSh", "createAllUserShortcut" }, "Create application shortcuts for all users. Option reserved when insllation and data location are not in user folder. Default value is true [createAllUserShortcut=false]", "true");

    QCommandLineParser parser;

    parser.setApplicationDescription("Stand-alone deployment. Set --help option to display help");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOption(silent_opt);
    parser.addOption(intallLocation_opt);
    parser.addOption(dataLocation_opt);
    parser.addOption(proxyUse_opt);
    parser.addOption(proxyAuto_opt);
    parser.addOption(proxyURL_opt);
    parser.addOption(proxyPort_opt);
    parser.addOption(proxyLogin_opt);
    parser.addOption(proxyPassword_opt);
    parser.addOption(language_opt);
    parser.addOption(runAtStart_opt);
    parser.addOption(createOfflineShortcut_opt);
    parser.addOption(createShortcut_opt);
    parser.addOption(createAllUserShortcut_opt);

    L_INFO("Start to parse command line arguments");

    parser.process(QCoreApplication::arguments());

    L_INFO("End to parse command line arguments");
    L_INFO("Start to read command line arguments with parse data");

    silent = isExistingBooleanArg(parser, silent_opt);
    intallLocation = parser.value(intallLocation_opt);
    dataLocation = parser.value(dataLocation_opt);
    proxyUse = isExistingBooleanArg(parser, proxyUse_opt);
    proxyAuto = isExistingBooleanArg(parser, proxyAuto_opt);
    proxyURL = parser.value(proxyURL_opt);
    proxyPort = parser.value(proxyPort_opt);
    proxyLogin = parser.value(proxyLogin_opt);
    proxyPassword = parser.value(proxyPassword_opt);
    language = parser.value(language_opt);
    runAtStart = isExistingBooleanArg(parser, runAtStart_opt);
    offshort = isExistingBooleanArg(parser, createOfflineShortcut_opt);
    shortcut = isExistingBooleanArg(parser, createShortcut_opt);
    allUserShortcut = isExistingBooleanArg(parser, createAllUserShortcut_opt);

    L_INFO("End to read command line arguments with parse data");
}

bool CommandLineParser::isExistingBooleanArg(QCommandLineParser & parser, QCommandLineOption & commandOption){
    return (parser.isSet(commandOption) && (parser.value(commandOption) == "" || parser.value(commandOption) == "true"));
}

QString CommandLineParser::getValueArg(QCommandLineParser & parser, QCommandLineOption & commandOption){
    QString value = parser.value(commandOption);

    if (value == "EMPTY") {
        value = "";
    }
    return value;
}

bool CommandLineParser::getAllUserShortcut() const {
    return allUserShortcut;
}

void CommandLineParser::setAllUserShortcut(bool value){
    allUserShortcut = value;
}

bool CommandLineParser::getShortcut() const {
    return shortcut;
}

void CommandLineParser::setShortcut(bool value){
    shortcut = value;
}

bool CommandLineParser::getOffshort() const {
    return offshort;
}

void CommandLineParser::setOffshort(bool value){
    offshort = value;
}

bool CommandLineParser::getRunAtStart() const {
    return runAtStart;
}

void CommandLineParser::setRunAtStart(bool value){
    runAtStart = value;
}

QString CommandLineParser::getLanguage() const {
    return language;
}

void CommandLineParser::setLanguage(const QString &value){
    language = value;
}

QString CommandLineParser::getProxyPassword() const {
    return proxyPassword;
}

void CommandLineParser::setProxyPassword(const QString &value){
    proxyPassword = value;
}

QString CommandLineParser::getProxyLogin() const {
    return proxyLogin;
}

void CommandLineParser::setProxyLogin(const QString &value){
    proxyLogin = value;
}

QString CommandLineParser::getProxyPort() const {
    return proxyPort;
}

void CommandLineParser::setProxyPort(const QString &value){
    proxyPort = value;
}

QString CommandLineParser::getProxyURL() const {
    return proxyURL;
}

void CommandLineParser::setProxyURL(const QString &value){
    proxyURL = value;
}

bool CommandLineParser::getProxyAuto() const {
    return proxyAuto;
}

void CommandLineParser::setProxyAuto(bool value){
    proxyAuto = value;
}

bool CommandLineParser::getProxyUse() const {
    return proxyUse;
}

void CommandLineParser::setProxyUse(bool value){
    proxyUse = value;
}

QString CommandLineParser::getDataLocation() const {
    return dataLocation;
}

void CommandLineParser::setDataLocation(const QString &value){
    dataLocation = value;
}

QString CommandLineParser::getIntallLocation() const {
    return intallLocation;
}

void CommandLineParser::setIntallLocation(const QString &value){
    intallLocation = value;
}

bool CommandLineParser::getSilent() const {
    return silent;
}

void CommandLineParser::setSilent(bool value){
    silent = value;
}
