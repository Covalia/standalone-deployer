#ifndef INSTALLER__PERSONALIZE_H
#define INSTALLER__PERSONALIZE_H

#include <QString>
#include <QWidget>
#include <QSharedDataPointer>

#include "settings/resourcessettings.h"

namespace Ui {
class PersonalizeUI;
}

/**
 * @class PersonalizeUI
 * @brief Personalize widget
 */
class PersonalizeUI : public QWidget
{
    Q_OBJECT

    public:
        explicit PersonalizeUI(QSharedPointer<ResourcesSettings> _resourcesSettings, QWidget * _parent = 0);
        virtual ~PersonalizeUI();

    private:
        QSharedPointer<ResourcesSettings> m_resourcesSettings;

        Ui::PersonalizeUI * m_ui;
        QString fileChooserDialog(QString directory);
        void saveElementsInSetting();

    signals:
        void proxySettingSignal();
        void customInstallationSignal();
        void installationFolderChanged(QString folder);

    public slots:
        void changeLanguage();
        void proxySettingEvent();
        void customInstallationEvent();
        void fileChooserInstallEvent();
        void fileChooserDataEvent();
};

#endif // INSTALLER__PERSONALIZE_H
