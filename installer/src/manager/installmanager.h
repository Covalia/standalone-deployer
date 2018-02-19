#ifndef INSTALLMANAGER_H
#define INSTALLMANAGER_H

#include <QString>
#include <QObject>
#include <QThread>

#include "gui/manager/uimanager.h"
#include "settings/resourcessettings.h"
#include "settings/settings.h"
#include "utils.h"

class InstallManager: public QThread
{
    Q_OBJECT

    public:
        InstallManager();
        virtual ~InstallManager();

        void initInstallation();

    private:

        UIManager * m_uiManager;
        AppPath m_appPath;
        ResourcesSettings * m_projectSettings;
        Settings * m_settings;

        bool runAppAfter;

        bool createInstallationFolders();
        void moveLogIntoInstallFolder();
        bool createIniConfigurationFile();
        bool createUpdaterFolderVersion();
        bool extractResources();
        bool createShortcut();
        void startInstallation();
        void closeInstallation(bool _launchApplication);
        bool launchLoader();

    protected:
         void run();

    private slots:
        void eventStartInstallation();
        void eventCloseInstallation(bool _launchApplication);

    signals:
        void endInstallation(bool _success, QString _error);
};

#endif // INSTALLMANAGER_H
