#include "gui/style/stylemanager.h"
#include "settings/resourcessettings.h"

#include <QApplication>
#include <QTextStream>
#include <QTextCursor>
#include <QFile>

StyleManager::StyleManager()
{
}

QString StyleManager::transformStyle(QString styleSheet)
{
    ResourcesSettings * projectSetting = ResourcesSettings::getInstance();

    styleSheet.replace("@inset-color", projectSetting->getInsetColor());
    styleSheet.replace("@panel-background-color", projectSetting->getPanelBackgroundColor());
    styleSheet.replace("@button-hover-background-color", projectSetting->getButtonHoverBackgroundColor());
    styleSheet.replace("@button-background-color", projectSetting->getButtonBackgroundColor());
    styleSheet.replace("@default-text-color", projectSetting->getDefaultTextColor());
    styleSheet.replace("@gray-text-color", projectSetting->getGrayTextColor());
    styleSheet.replace("@disabled-color", projectSetting->getDisabledColor());
    styleSheet.replace("@window-border-width", projectSetting->getWindowBorderWidth());

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
    QFile f(":/style.css");

    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        QString sytle = in.readAll();
        if (!sytle.isNull() && !sytle.isEmpty()) {
            QString transformeSytle = transformStyle(sytle);
            qApp->setStyleSheet(transformeSytle);
        }
    }
}
