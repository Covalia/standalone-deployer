#include "stylemanager.h"

StyleManager::StyleManager(){
}

QString StyleManager::transformStyle(QString styleSheet){
    styleSheet.replace("@color-panel-background", "#2d364c");
    styleSheet.replace("@color-button-background", "#339999");
    styleSheet.replace("@color-text-on-background", "#eff0f2");
    styleSheet.replace("@color-text-gray", "#9ea0a5");

    return styleSheet;
}

void StyleManager::transformStyle(QWidget * parentWidget){
    for(auto *widget : parentWidget->findChildren<QWidget *>()) {
        widget->setStyleSheet(StyleManager::transformStyle(widget->styleSheet()));
    }
}
