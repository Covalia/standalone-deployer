#ifndef INSTALLMANAGER_H
#define INSTALLMANAGER_H

#include <QString>
#include <QObject>

#include "gui/manager/uimanager.h"
#include "fs/apptreemanager.h"

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
};

#endif // INSTALLMANAGER_H
