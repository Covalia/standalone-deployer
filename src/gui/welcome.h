#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget * parent = 0);
    virtual ~Welcome();

signals:
    void customInstallationSignal();

public slots:
    void changeLanguage();
    void customInstallationEvent();

private:
    Ui::Welcome * ui;
};

#endif // WELCOME_H
