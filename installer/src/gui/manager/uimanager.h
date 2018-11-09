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
        UIManager(const QString &_appName);
        virtual ~UIManager();
        void init();

        bool isCustomInstallation() const;
        QString getInstallationFolder() const;
        bool isChosenDataFolder() const;
        QString getDataFolder() const;
        bool isCreatedOfflineShortcut() const;
        bool isLaunchedAppAtStartUp() const;
        bool isUsedProxy() const;
        QString getProxyHostname() const;
        quint16 getProxyPort() const;
        QString getProxyLogin() const;
        QString getProxyPassword() const;
        bool isStartedAppWhenInstalled() const;

        void setInstallationFolder(const QString _installationFolder);
        void setDataFolder(const QString _dataFolder);
        void setCreatedOfflineShortcut(const bool _createdOfflineShortcut);
        void setLaunchedAppAtStartUp(const bool _launchedAppAtStartUp);
        void setStartedAppWhenInstalled(const bool _startedAppWhenInstalled);

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
