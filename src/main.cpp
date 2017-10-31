#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QDebug>
#include "gui/mainwindow.h"

int main(int argc, char *argv[])
{
	qDebug() << "Starting application";
    QApplication app(argc, argv);

    QTranslator translator;
    // TODO gérer langue par défaut et demander quelle langue utiliser.
    QString locale = QLocale::system().name();
    QString location = ":/translations";
    qDebug() << "locale:" << locale;

    qDebug() << "loading translator";
    translator.load(locale, location);
    app.installTranslator(&translator);

    MainWindow window;
    window.show();
    window.center();

    return app.exec();
}
