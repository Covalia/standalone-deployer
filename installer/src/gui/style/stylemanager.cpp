#include "gui/style/stylemanager.h"
#include "settings/resourcesettings.h"

#include <QApplication>
#include <QTextStream>
#include <QFile>
#include <QWidget>

QString StyleManager::transformStyle(QSharedPointer<ResourceSettings> _resourceSettings, QString _styleSheet)
{
    _styleSheet.replace("@inset-color", _resourceSettings->getInsetColor());
    _styleSheet.replace("@panel-background-color", _resourceSettings->getPanelBackgroundColor());
    _styleSheet.replace("@button-hover-background-color", _resourceSettings->getButtonHoverBackgroundColor());
    _styleSheet.replace("@button-background-color", _resourceSettings->getButtonBackgroundColor());
    _styleSheet.replace("@default-text-color", _resourceSettings->getDefaultTextColor());
    _styleSheet.replace("@gray-text-color", _resourceSettings->getGrayTextColor());
    _styleSheet.replace("@disabled-color", _resourceSettings->getDisabledColor());
    _styleSheet.replace("@window-border-width", _resourceSettings->getWindowBorderWidth());

    return _styleSheet;
}

void StyleManager::transformStyle(QSharedPointer<ResourceSettings> _resourceSettings, QWidget * parentWidget)
{
    for (auto * widget : parentWidget->findChildren<QWidget *>()) {
        widget->setStyleSheet(StyleManager::transformStyle(_resourceSettings, widget->styleSheet()));
    }
}

void StyleManager::setGeneralStyle(QSharedPointer<ResourceSettings> _resourceSettings)
{
    QFile f(":/style.css");

    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        const QString style = in.readAll();
        if (!style.isNull() && !style.isEmpty()) {
            const QString transformeSytle = transformStyle(_resourceSettings, style);
            qApp->setStyleSheet(transformeSytle);
        }
    }
}
