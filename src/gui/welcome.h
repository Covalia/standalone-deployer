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
    void simpleInstallationSignal();
    void customInstallationSignal();
    void contractSignal();

public slots:
    void changeLanguage();
    void simpleInstallationEvent();
    void customInstallationEvent();
    void customContractEvent();

private:
    Ui::Welcome * ui;
};

#endif // WELCOME_H
