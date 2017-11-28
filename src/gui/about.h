#ifndef ABOUT_H
#define ABOUT_H

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
    explicit About(QWidget * parent = 0);
    ~About();

private:
    Ui::About * ui;
    void initTextContract();

signals:
    void validateAboutSignal();

public slots:
    void changeLanguage();
    void validateAbout();
};

#endif // ABOUT_H
