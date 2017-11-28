#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QWidget>

class Windows;
class Welcome;
class Personnalize;
class Proxy;
class About;
class DownloadUI;

class UIManager : public QObject
{
    Q_OBJECT

    public:
        UIManager();
        virtual ~UIManager();

        void changeTo(QWidget * widget);
        void changeWelcome();
        void changePersonnalize();
        void changeProxy();
        void changeAbout();
        void changeDownload();

        void removeWelcome();
        void removePersonnalize();
        void removeProxy();
        void removeAbout();
        void removeDownload();

    private:
        Windows * m_window;
        Welcome * m_welcome;
        Personnalize * m_personnalize;
        Proxy * m_proxy;
        About * m_about;
        DownloadUI * m_download;

        QWidget * m_saveWidget;

    private slots:
        void aboutEvent();
        void switchWelcomToPersonnalize();
        void switchWelcomToDownload();
        void switchPersonnalizeToProxy();
        void switchPersonnalizeToDownload();
        void switchProxyToPersonnalize();
        void switchAboutTo();
};

#endif // UIMANAGER_H
