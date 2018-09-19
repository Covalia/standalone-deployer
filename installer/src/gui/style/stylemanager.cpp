#include "gui/style/stylemanager.h"
#include "settings/resourcessettings.h"

#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QWidget>

QString StyleManager::transformStyle(QSharedPointer<ResourcesSettings> _resourcesSettings, QString _styleSheet)
{
    _styleSheet.replace("@inset-color", _resourcesSettings->getInsetColor());
    _styleSheet.replace("@panel-background-color", _resourcesSettings->getPanelBackgroundColor());
    _styleSheet.replace("@button-hover-background-color", _resourcesSettings->getButtonHoverBackgroundColor());
    _styleSheet.replace("@button-background-color", _resourcesSettings->getButtonBackgroundColor());
    _styleSheet.replace("@default-text-color", _resourcesSettings->getDefaultTextColor());
    _styleSheet.replace("@gray-text-color", _resourcesSettings->getGrayTextColor());
    _styleSheet.replace("@disabled-color", _resourcesSettings->getDisabledColor());
    _styleSheet.replace("@window-border-width", _resourcesSettings->getWindowBorderWidth());

    return _styleSheet;
}

void StyleManager::transformStyle(QSharedPointer<ResourcesSettings> _resourcesSettings, QWidget * parentWidget)
{
    for (auto * widget : parentWidget->findChildren<QWidget *>()) {
        widget->setStyleSheet(StyleManager::transformStyle(_resourcesSettings, widget->styleSheet()));
    }
}

void StyleManager::setGeneralStyle(QSharedPointer<ResourcesSettings> _resourcesSettings)
{
    QFile f(":/style.css");

    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        const QString style = in.readAll();
        if (!style.isNull() && !style.isEmpty()) {
            const QString transformeSytle = transformStyle(_resourcesSettings, style);
            qApp->setStyleSheet(transformeSytle);
        }
    }
}
