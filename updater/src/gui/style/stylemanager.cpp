#include "gui/style/stylemanager.h"
#include "utils.h"

#include <QApplication>
#include <QTextStream>
#include <QFile>

void StyleManager::setGeneralStyle(const QString &_styleName)
{
    AppPath appPath = Utils::getAppPath();

    QFile f(QString(":/styles/%1.css").arg(_styleName));

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
