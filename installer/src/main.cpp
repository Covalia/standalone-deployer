#include <QApplication>
#include <QDebug>

#include "log/logger.h"
#include "shortcut/shortcut.h"
#include "commandline/commandlineparser.h"

int main(int argc, char * argv[])
{
	qDebug() << "installer started";
    QApplication app(argc, argv);

    L_INFO("Start Commande Lien Parser");
    CommandLineParser * lineParser = new CommandLineParser();
    lineParser->sendToSettings();

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

    return app.exec();
}
