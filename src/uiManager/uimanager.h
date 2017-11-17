#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>

class Windows;
class Welcome;
class Personnalize;
class Proxy;
class About;

class UIManager : public QObject
{
    Q_OBJECT

public:
    UIManager();
    virtual ~UIManager();

    void changeWelcome();
    void changePersonnalize();
    void changeProxy();
    void changeAbout();

    void removeWelcome();
    void removePersonnalize();
    void removeProxy();
    void removeAbout();

private:
    Windows * m_window;
    Welcome * m_welcome;
    Personnalize * m_personnalize;
    Proxy * m_proxy;
    About * m_about;

private slots:
    void aboutEvent();
    void switchWelcomToPersonnalize();
    void switchPersonnalizeToProxy();
    void switchProxyToPersonnalize();
};

#endif // UIMANAGER_H
