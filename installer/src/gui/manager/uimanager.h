#ifndef INSTALLER__UIMANAGER_H
#define INSTALLER__UIMANAGER_H

#include <QObject>
#include <QWidget>

class WindowUI;
class InstallationUI;
class EndInstallationUI;
class InstallWizard;
class AbstractTranslatedUi;

class UIManager : public QObject
{
    Q_OBJECT

    public:
        UIManager(const QString &_appName, bool _changeDataLocationAllowed);
        virtual ~UIManager();
        void init(const QString &_locale);

        bool isCustomInstallation() const;
        QString getInstallationFolder() const;
        bool isDataFolderChosen() const;
        void setDataFolderChosen(bool _folderChosen);
        QString getDataFolder() const;
        bool isCreatedOfflineShortcut() const;
        bool isLaunchedAppAtStartUp() const;
        bool isProxyUsed() const;
        QString getProxyHostname() const;
        quint16 getProxyPort() const;
        QString getProxyLogin() const;
        QString getProxyPassword() const;
        bool isStartedAppWhenInstalled() const;

        void setInstallationFolder(const QString &_installationFolder);
        void setDataFolder(const QString &_dataFolder);
        void setCreatedOfflineShortcut(const bool _createdOfflineShortcut);
        void setLaunchedAppAtStartUp(const bool _launchedAppAtStartUp);
        void setStartedAppWhenInstalled(const bool _startedAppWhenInstalled);

        void setProxyUsed(bool _proxyUsed);
        void setProxyHostname(const QString &_hostname);
        void setProxyPort(quint16 _port);
        void setProxyLogin(const QString &_login);
        void setProxyPassword(const QString _password);

        void setCustomInstallation(bool _customInstallation);

        void printWizard() const;

        QString getLocale() const;

    private slots:
        void wizardFinished(int _result);
        void languageChanged(const QString &_language);

    private:
        WindowUI * m_window;
        InstallationUI * m_installation;
        EndInstallationUI * m_endInstallation;
        InstallWizard * m_wizard;

        AbstractTranslatedUi * m_currentWidget;

    public slots:
        void eventEndInstallation(bool _success, QStringList _errors);

    signals:
        void wizardFinishedSignal();
        void quitInstaller(bool _startApp);
};

#endif // INSTALLER__UIMANAGER_H
