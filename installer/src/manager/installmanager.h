#ifndef INSTALLMANAGER_H
#define INSTALLMANAGER_H

#include <QString>
#include <QObject>
#include <QThread>
#include <QSharedPointer>

#include "gui/manager/uimanager.h"
#include "settings/resourcesettings.h"
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

        bool m_runAppAfter;

        QString m_installationPath;
        QString m_dataPath;

        bool cleanInstallationFolders();
        bool createInstallationFolders();
        void moveLogIntoInstallFolder();
        bool createIniConfigurationFile();
        bool createUpdaterFolderVersion();
        bool extractResources();
        bool createShortcut();
        void startInstallation();
        bool launchLoader();

        void setInstallationDir(const QString &_directory);

    protected:
        virtual void run();

    private slots:
        void closeInstallation(bool _launchApplication);

    signals:
        void endInstallation(bool _success, QStringList _errors);
};

#endif
