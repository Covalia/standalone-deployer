#ifndef INSTALLMANAGER_H
#define INSTALLMANAGER_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QSharedPointer>

#include "gui/manager/uimanager.h"
#include "settings/resourcesettings.h"
#include "commandline/commandlineparser.h"
#include "settings/settings.h"
#include "utils.h"

class InstallManager : public QThread
{
    Q_OBJECT

    public:
        InstallManager();
        virtual ~InstallManager();

        void initInstallation();

    private:

        UIManager * m_uiManager;
        AppPath m_appPath;
        QSharedPointer<ResourceSettings> m_projectSettings;
        Settings * m_settings;

        CommandLineParser m_lineParser;

        // temporary values
        QString m_installLocation;
        QString m_dataLocation;
        bool m_proxyUsed;
        QString m_proxyHostname;
        QString m_proxyPort;
        QString m_proxyLogin;
        QString m_proxyPassword;
        bool m_runApp;
        bool m_runAtStart;
        bool m_createOfflineShortcut;

        bool cleanInstallationFolders();
        bool createInstallationFolders();
        void moveLogIntoInstallFolder();
        bool createIniConfigurationFile();
        bool createUpdaterFolderVersion();
        bool extractResources();
        bool createShortcut();
        void startInstallation();
        bool launchLoader();

    protected:
        virtual void run();

    private slots:
        void closeInstallation(bool _launchApplication);

    signals:
        void endInstallation(bool _success, QStringList _errors);
};

#endif
