#include "gui/style/stylemanager.h"
#include "utils.h"

#include <QApplication>
#include <QTextStream>
#include <QFile>

void StyleManager::setGeneralStyle()
{
    AppPath appPath = Utils::getAppPath();

    QFile f(appPath.getConfigurationDir().absoluteFilePath("style.css"));

    if (f.exists()) {
        if (f.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&f);
            QString style = in.readAll();
            if (!style.isNull() && !style.isEmpty()) {
                qApp->setStyleSheet(style);
            }
        }
    }
}
