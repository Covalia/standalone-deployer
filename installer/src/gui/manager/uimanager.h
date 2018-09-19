#ifndef INSTALLER__UIMANAGER_H
#define INSTALLER__UIMANAGER_H

#include <QObject>
#include <QWidget>
#include <QSharedDataPointer>

#include "settings/resourcessettings.h"

class WindowUI;
class WelcomeUI;
class PersonalizeUI;
class ProxyUI;
class AboutUI;
class InstallationUI;
class EndInstallationUI;

class UIManager : public QObject
{
    Q_OBJECT

    public:
        UIManager(QSharedPointer<ResourcesSettings> _resourcesSettings);
        virtual ~UIManager();
        void init();

    private:
        QSharedPointer<ResourcesSettings> m_resourcesSettings;
        WindowUI * m_window;
        WelcomeUI * m_welcome;
        PersonalizeUI * m_personalize;
        ProxyUI * m_proxy;
        AboutUI * m_about;
        InstallationUI * m_installation;
        EndInstallationUI * m_endInstallation;

        enum Page { WelcomePage, PersonalizePage, ProxyPage, AboutPage, InstallationPage, EndInstallationPage };
        Page m_returnPage;

        void returnToLastPage();
        void changeWelcome();
        void changePersonalize();
        void changeProxy();
        void changeAbout();
        void changeInstallation();
        void changeEndInstallation();

    private slots:
        void aboutEvent();
        void switchWelcomeToPersonalize();
        void switchWelcomeToInstallation();
        void switchPersonalizeToProxy();
        void switchPersonalizeToInstallation();
        void switchProxyToPersonalize();
        void switchAboutTo();
        void eventCloseInstallation(bool _launchApplication);
        void installationFolderChangedEvent(QString _folder);

    public slots:
        void eventEndInstallation(bool _success, QStringList _errors);

    signals:
        void changeInstallationSignal();
        void closeInstallationSignal(bool _launchApplication);
        void installationFolderChanged(QString _folder);
};

#endif // INSTALLER__UIMANAGER_H
