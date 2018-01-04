#ifndef INSTALLER__WELCOME_H
#define INSTALLER__WELCOME_H

#include <QWidget>

namespace Ui {
class WelcomeUI;
}

/**
 * @class WelcomeUI
 * @brief Welcome widget
 */
class WelcomeUI : public QWidget
{
    Q_OBJECT

    public:
        explicit WelcomeUI(QWidget * _parent = 0);
        virtual ~WelcomeUI();

    signals:
        void simpleInstallationSignal();
        void customInstallationSignal();
        void contractSignal();

    public slots:
        void changeLanguage();
        void simpleInstallationEvent();
        void customInstallationEvent();
        void customContractEvent();

    private:
        Ui::WelcomeUI * m_ui;
};

#endif // INSTALLER__WELCOME_H
