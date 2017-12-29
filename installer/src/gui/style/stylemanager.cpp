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

    styleSheet.replace("@color-panel-background-border", projectSetting->getColor_panel_background_border());
    styleSheet.replace("@color-panel-background", projectSetting->getColor_panel_background());
    styleSheet.replace("@color-button-background-over", projectSetting->getColor_button_background_over());
    styleSheet.replace("@color-button-background", projectSetting->getColor_button_background());
    styleSheet.replace("@color-text-on-background", projectSetting->getColor_text_on_background());
    styleSheet.replace("@color-text-gray", projectSetting->getColor_text_gray());
    styleSheet.replace("@color-disabled", projectSetting->getColor_disabled());
    styleSheet.replace("@border-window", projectSetting->getBorder_window());

    return styleSheet;
} // StyleManager::transformStyle

void StyleManager::transformStyle(QWidget * parentWidget)
{
    for (auto * widget : parentWidget->findChildren<QWidget *>()) {
        widget->setStyleSheet(StyleManager::transformStyle(widget->styleSheet()));
    }
}

void StyleManager::setGeneralStyle(){
    QFile f(":/resources/style.css");

    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        QString sytle = in.readAll();
        if(!sytle.isNull() && !sytle.isEmpty()){
            QString transformeSytle = transformStyle(sytle);
            qApp->setStyleSheet(transformeSytle);
        }
    }
}
