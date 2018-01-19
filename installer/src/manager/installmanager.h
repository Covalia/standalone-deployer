#ifndef INSTALLMANAGER_H
#define INSTALLMANAGER_H

#include <QString>
#include <QObject>

#include "gui/manager/uimanager.h"
#include "fs/apptreemanager.h"
#include "settings/resourcessettings.h"
#include "settings/settings.h"

class InstallManager: public QObject
{
    Q_OBJECT

    public:
        InstallManager();
        virtual ~InstallManager();

        void initInstallation();

    private:

        UIManager * m_uiManager;
        AppTreeManager * m_treeManager;
        ResourcesSettings * m_projectSettings;
        Settings * m_settings;

        bool runAppAfter;

        bool createInstallationFolders();
        bool createIniConfigurationFile();
        bool extractResources();
        bool createDesktopShortcut();
        void startInstallation();
        void closeInstallation(bool _launchApplication);
        bool launchLoader();

    private slots:
        void eventStartInstallation();
        void eventCloseInstallation(bool _launchApplication);

    signals:
        void endInstallation(bool _success, QString _error);
};

#endif // INSTALLMANAGER_H
