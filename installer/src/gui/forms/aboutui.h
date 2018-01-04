#ifndef INSTALLER__ABOUT_H
#define INSTALLER__ABOUT_H

#include <QWidget>

namespace Ui {
class AboutUI;
}

/**
 * @class AboutUI
 * @brief About widget
 */
class AboutUI : public QWidget
{
    Q_OBJECT

    public:
        explicit AboutUI(QWidget * _parent = 0);
        virtual ~AboutUI();

    private:
        Ui::AboutUI * m_ui;
        void initTextContract();

    signals:
        void validateAboutSignal();

    public slots:
        void changeLanguage();
        void validateAbout();
};

#endif // INSTALLER__ABOUT_H
