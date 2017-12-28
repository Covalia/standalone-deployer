#ifndef INSTALLER__PERSONALIZE_H
#define INSTALLER__PERSONALIZE_H

#include <QWidget>
#include <QString>

namespace Ui {
class Personalize;
}

/**
 * @class Personalize
 * @brief Personalize widget
 */
class Personalize : public QWidget
{
    Q_OBJECT

    public:
        explicit Personalize(QWidget * _parent = 0);
        virtual ~Personalize();

    private:
        Ui::Personalize * m_ui;
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
