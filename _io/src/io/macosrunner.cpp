#include "io/macosrunner.h"
#include <QProcess>
#include <QStringList>
#include <QtDebug>
#include "log/logger.h"

bool MacosRunner::openDmgFile(const QString &_appName)
{
    L_INFO(">>>>> openDmgFile");
    QStringList args;

    // TODO le dossier doit exister
    // créer une nouvelle constante spécifique à macos pour le dossier Mount
    args << "attach";
    args << "-noautoopen";
    args << "-mountroot";
    args << "./tmp/";
    // TODO gérer le chemin de l'appli en fonction du nom de l'appli, utiliser une enum ?
    args << "/Users/matthieu/Qt-Projects/standalone-deployer/updater/bin/updater.dmg";

    QProcess process;
    process.start("hdiutil", args);
    return process.waitForFinished();
}

bool MacosRunner::closeDmgFile(const QString &_appName)
{
    L_INFO(">>>>> closeDmgFile");

    QStringList args;
    // TODO mêmes remarques que ci-dessus
    args << "detach";
    args << "./tmp/updater"; // le nom de l'image

    QProcess process;
    process.start("hdiutil", args);
    return process.waitForFinished();
}

bool MacosRunner::openApp(const QString &_appName, const QStringList &_args)
{
    // TODO mêmes remarques que ci-dessus

    QProcess process;
    // open -a /usr/local/Cellar/macvim/8.0-143/MacVim.app --args /etc/passwd
    QStringList args;

    args << "-a";
    args << "/usr/local/Cellar/macvim/8.0-143/MacVim.app";
    if (_args.size() > 0) {
        // les arguments doivent être passés après l'argument nommé --args
        args << "--args";
        args << _args;
    }

    process.start("open", args);
    // open retourne lorsque le programme est lancé, donc on peut faire un waitForFinished :)
    return process.waitForFinished();
}
