#ifndef INSTALLER__ABOUT_H
#define INSTALLER__ABOUT_H

#include <QWidget>

namespace Ui {
class About;
}

/**
 * @class About
 * @brief About widget
 */
class About : public QWidget
{
    Q_OBJECT

    public:
        explicit About(QWidget * _parent = 0);
        virtual ~About();

    private:
        Ui::About * m_ui;
        void initTextContract();

    signals:
        void validateAboutSignal();

    public slots:
        void changeLanguage();
        void validateAbout();
};

#endif // ABOUT_H
