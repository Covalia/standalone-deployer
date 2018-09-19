#ifndef INSTALLER__PERSONALIZE_H
#define INSTALLER__PERSONALIZE_H

#include <QString>
#include <QWidget>
#include <QSharedDataPointer>

#include "settings/resourcesettings.h"

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
        explicit PersonalizeUI(QSharedPointer<ResourceSettings> _resourceSettings, QWidget * _parent = 0);
        virtual ~PersonalizeUI();

    private:
        QSharedPointer<ResourceSettings> m_resourceSettings;

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
