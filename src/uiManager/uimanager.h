#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QWidget>

class Window;
class Welcome;
class Personalize;
class Proxy;
class About;
class DownloadUI;

class UIManager : public QObject
{
    Q_OBJECT

    public:
        UIManager();
        virtual ~UIManager();
        void init();

    private:
        Window * m_window;
        Welcome * m_welcome;
        Personalize * m_personalize;
        Proxy * m_proxy;
        About * m_about;
        DownloadUI * m_download;

        enum Page { WelcomePage, PersonalizePage, ProxyPage, AboutPage, DownloadPage };
        Page m_returnPage;

        void returnToLastPage();
        void changeWelcome();
        void changePersonalize();
        void changeProxy();
        void changeAbout();
        void changeDownload();

    private slots:
        void aboutEvent();
        void switchWelcomeToPersonalize();
        void switchWelcomeToDownload();
        void switchPersonalizeToProxy();
        void switchPersonalizeToDownload();
        void switchProxyToPersonalize();
        void switchAboutTo();
};

#endif // UIMANAGER_H
