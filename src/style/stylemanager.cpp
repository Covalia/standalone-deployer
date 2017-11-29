#include "stylemanager.h"

StyleManager::StyleManager()
{
}

QString StyleManager::transformStyle(QString styleSheet)
{
    // blue avast
//    styleSheet.replace("@color-panel-background-border", "#364058");
//    styleSheet.replace("@color-panel-background", "#2d364c");
//    styleSheet.replace("@color-button-background-over", "#2a7d7d");
//    styleSheet.replace("@color-button-background", "#339999");
//    styleSheet.replace("@color-text-on-background", "#eff0f2");
//    styleSheet.replace("@color-text-gray", "#9ea0a5");
//    styleSheet.replace("@color-disabled", "#656976");
//    styleSheet.replace("@border-window", "0");

    // pink
    styleSheet.replace("@color-panel-background-border", "#e6176c");
    styleSheet.replace("@color-panel-background", "#e66fa0");
    styleSheet.replace("@color-button-background-over", "#d43176");
    styleSheet.replace("@color-button-background", "#a41f57");
    styleSheet.replace("@color-text-on-background", "#ffffff");
    styleSheet.replace("@color-text-gray", "#d2d2d2");
    styleSheet.replace("@color-disabled", "#b1a8a8");
    styleSheet.replace("@border-window", "1");

    // turquoise
//    styleSheet.replace("@color-panel-background-border", "#007688");
//    styleSheet.replace("@color-panel-background", "#208275");
//    styleSheet.replace("@color-button-background-over", "#12bda7");
//    styleSheet.replace("@color-button-background", "#07a28e");
//    styleSheet.replace("@color-text-on-background", "#eff0f2");
//    styleSheet.replace("@color-text-gray", "#d2d2d2");
//    styleSheet.replace("@color-disabled", "#656976");
//    styleSheet.replace("@border-window", "0");

    // white
//    styleSheet.replace("@color-panel-background-border", "#EEEEEE");
//    styleSheet.replace("@color-panel-background", "#FFFFFF");
//    styleSheet.replace("@color-button-background-over", "#12bda7");
//    styleSheet.replace("@color-button-background", "#07a28e");
//    styleSheet.replace("@color-text-on-background", "#222222");
//    styleSheet.replace("@color-text-gray", "#555555");
//    styleSheet.replace("@color-disabled", "#bbbbbb");
//    styleSheet.replace("@border-window", "1");

    return styleSheet;
} // StyleManager::transformStyle

void StyleManager::transformStyle(QWidget * parentWidget)
{
    for (auto * widget : parentWidget->findChildren<QWidget *>()) {
        widget->setStyleSheet(StyleManager::transformStyle(widget->styleSheet()));
    }
}
