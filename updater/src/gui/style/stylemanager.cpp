#include "gui/style/stylemanager.h"
#include "settings/settings.h"
#include "fs/apptreemanager.h"
#include "utils.h"

#include <QApplication>
#include <QTextStream>
#include <QTextCursor>
#include <QFile>
#include <QSettings>

StyleManager::StyleManager()
{
}

QString StyleManager::transformStyle(QString styleSheet)
{
    Settings * settings = Settings::getInstance();

    styleSheet.replace("@color-panel-background-border", settings->getColorPanelBackgroundBorder());
    styleSheet.replace("@color-panel-background", settings->getColorPanelBackground());
    styleSheet.replace("@color-button-background-over", settings->getColorButtonBackgroundOver());
    styleSheet.replace("@color-button-background", settings->getColorButtonBackground());
    styleSheet.replace("@color-text-on-background", settings->getColorTextOnBackground());
    styleSheet.replace("@color-text-gray", settings->getColorTextGray());
    styleSheet.replace("@color-disabled", settings->getColorDisabled());
    styleSheet.replace("@border-window", settings->getBorderWindow());

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
    InstallPath installPath = Utils::getInstallPath();

    QFile f(installPath.getConfigurationDirPath().absolutePath() + "/style.css");

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
