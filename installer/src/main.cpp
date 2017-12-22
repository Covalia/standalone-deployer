#include <QApplication>
#include <QDebug>
#include <QIcon>

#include "log/logger.h"
#include "shortcut/shortcut.h"
#include "commandline/commandlineparser.h"
#include "settings/settings.h"
#include "settings/resourcessettings.h"
#include "language/languagemanager.h"
#include "uimanager/uimanager.h"
#include "style/stylemanager.h"

int main(int argc, char * argv[])
{
    // TODO set path installation? Must be init before installation to log
    // logger initialization
    new Logger("installer.log");
    L_INFO("Installer started");

    QApplication app(argc, argv);

    L_INFO("Parsing command line");
    CommandLineParser * lineParser = new CommandLineParser();
    lineParser->sendToSettings();

    // init style
    StyleManager::setGeneralStyle();

    //init project resources
    ResourcesSettings * projectSettings = ResourcesSettings::getInstance();
    projectSettings->initSettings(":/resources/project.ini");
    projectSettings->readSettings();
    projectSettings->sendToSettings();
    L_INFO("Ip=" + projectSettings->getIp_server());

    // TODO remove this in end of installation manager
#ifdef _WIN32
        Shortcut * shorcutCreator = new Shortcut();
        LPCWSTR target_file = L"C:/Program Files (x86)/Opera/launcher.exe";
        LPCWSTR target_args = L"";
        LPSTR link_file = "C:/Users/Alexis/Desktop/covotem.lnk";
        LPCWSTR description = L"This is description";
        LPCWSTR cur_dir = L"C:/Users/Alexis/Desktop/";
        LPCWSTR icon_file = L"C:/Users/Alexis/Desktop/ido.ico";
        shorcutCreator->createWindowsShortcut(target_file, target_args,
                                              link_file, description,
                                              1, cur_dir,
                                              icon_file, 0);
        QString allUserStartMenu = shorcutCreator->findAllUserStartMenuFolder();
        QString userStartMenu = shorcutCreator->findUserStartMenuFolder();
        L_INFO(userStartMenu);
#endif

    // TODO set path of installation, and remove in installation manager
    Settings * settings = Settings::getInstance();
    QString installPath("standalone-deployer.ini");
    settings->initSettings(installPath);
    settings->setProxyPassword("hello@password!étoile?*");
    settings->writeSettings();
    settings->readSettings();
	// TODO ne pas logger le mdp
    L_INFO("Password = " + settings->getProxyPassword());

    app.setWindowIcon(QIcon(":/images/icon.png"));
    app.setApplicationName(QString(QObject::tr("Stand-alone deployment")));

    LanguageManager::initLanguage();

    UIManager * uiManager = new UIManager();
    uiManager->init();

    return app.exec();
} // main
