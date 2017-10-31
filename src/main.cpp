#include <QApplication>
#include <QDebug>
#include "mainwindow.h"
#include "shortcut/shortcut.h"


int main(int argc, char *argv[])
{
	qDebug() << "Starting application";
	QApplication a(argc, argv);

    Shortcut *shorcutCreator = new Shortcut();
    LPCWSTR target_file = L"C:/Program Files (x86)/Opera/launcher.exe";
    LPCWSTR target_args = L"";
    LPSTR link_file = "C:/Users/Alexis/Desktop/mylink.lnk";
    LPCWSTR description = L"This is description";
    LPCWSTR cur_dir = L"C:/Users/Alexis/Desktop/";
    LPCWSTR icon_file = L"C:/Users/Alexis/Desktop/ido.ico";

    shorcutCreator->createWindowsShortcut(target_file, target_args,
                                   link_file, description,
                                   1, cur_dir,
                                   icon_file, 0);

	MainWindow w;
	w.show();
	w.center();

	return a.exec();
}
