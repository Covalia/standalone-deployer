#ifndef INSTALLER__UIMANAGER_H
#define INSTALLER__UIMANAGER_H

#include <QObject>
#include <QWidget>

class WindowUI;
class WelcomeUI;
class PersonalizeUI;
class ProxyUI;
class AboutUI;
class DownloadUI;

class UIManager : public QObject
{
    Q_OBJECT

    public:
        UIManager();
        virtual ~UIManager();
        void init();

    private:
        WindowUI * m_window;
        WelcomeUI * m_welcome;
        PersonalizeUI * m_personalize;
        ProxyUI * m_proxy;
        AboutUI * m_about;
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
