#ifndef INSTALLMANAGER_H
#define INSTALLMANAGER_H

#include <QString>
#include <QObject>

#include "gui/manager/uimanager.h"

class InstallManager: public QObject
{
    Q_OBJECT

    public:
        InstallManager();
        virtual ~InstallManager();

        void initInstallation();

    private:

        UIManager * m_uiManager;
        bool runAppAfter;

        bool createInstallationFolders(QString installDir);
        bool createIniConfigurationFile();
        bool extractResources();
        bool createDesktopShortcut();
        void startInstallation();
        void closeInstallation(bool launchApplication);
        bool launchLoader();

    private slots:
        void eventStartInstallation();
        void eventCloseInstallation(bool launchApplication);
};

#endif // INSTALLMANAGER_H
