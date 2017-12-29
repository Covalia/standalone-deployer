#ifndef INSTALLER__PERSONALIZE_H
#define INSTALLER__PERSONALIZE_H

#include <QString>
#include <QWidget>

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
        explicit PersonalizeUI(QWidget * _parent = 0);
        virtual ~PersonalizeUI();

    private:
        Ui::PersonalizeUI * m_ui;
        QString fileChooserDialog(QString directory);
        void saveElementsInSetting();

    signals:
        void proxySettingSignal();
        void customInstallationSignal();

    public slots:
        void changeLanguage();
        void proxySettingEvent();
        void customInstallationEvent();
        void fileChooserInstallEvent();
        void fileChooserDataEvent();
};

#endif // PERSONALIZE_H
