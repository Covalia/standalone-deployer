#include "gui/style/stylemanager.h"
#include "settings/settings.h"
#include "utils.h"

#include <QApplication>
#include <QTextStream>
#include <QTextCursor>
#include <QFile>
#include <QWidget>

QString StyleManager::transformStyle(QString styleSheet)
{
    Settings * settings = Settings::getInstance();

    styleSheet.replace("@inset-color", settings->getInsetColor());
    styleSheet.replace("@panel-background-color", settings->getPanelBackgroundColor());
    styleSheet.replace("@button-hover-background-color", settings->getButtonHoverBackgroundColor());
    styleSheet.replace("@button-background-color", settings->getButtonBackgroundColor());
    styleSheet.replace("@default-text-color", settings->getDefaultTextColor());
    styleSheet.replace("@gray-text-color", settings->getGrayTextColor());
    styleSheet.replace("@disabled-color", settings->getDisabledColor());
    styleSheet.replace("@window-border-width", settings->getWindowBorderWidth());

    return styleSheet;
}

void StyleManager::transformStyle(QWidget * parentWidget)
{
    for (auto * widget : parentWidget->findChildren<QWidget *>()) {
        widget->setStyleSheet(StyleManager::transformStyle(widget->styleSheet()));
    }
}

void StyleManager::setGeneralStyle()
{
    AppPath appPath = Utils::getAppPath();

    QFile f(appPath.getConfigurationDir().absoluteFilePath("style.css"));

    if (f.exists()) {
        if (f.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&f);
            QString sytle = in.readAll();
            if (!sytle.isNull() && !sytle.isEmpty()) {
                QString transformeSytle = transformStyle(sytle);
                qApp->setStyleSheet(transformeSytle);
            }
        }
    }
}
