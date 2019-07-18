#ifndef INSTALLER_RESOURCESETTINGS_H
#define INSTALLER_RESOURCESETTINGS_H

#include <QString>
#include <QSettings>

class ResourceSettings
{
    public:

        static const QString DeploymentUrl;
        static const QString AppName;
        static const QString DefaultLocale;
        static const QString ShortcutName;
        static const QString RunAtStart;
        static const QString DefaultInstallationPath;
        static const QString ChangeDataLocationAllowed;

        ResourceSettings(const QString &_appPath);
        virtual ~ResourceSettings();

        void readSettings();

        bool isChangeDataLocationAllowed() const;
        QString getDefaultInstallationPath() const;
        QString getShortcutName() const;
        QString getDefaultLocale() const;
        QString getAppName() const;
        QString getDeploymentUrl() const;
        bool isRunAtStart() const;

    private:

        void operator=(const ResourceSettings&);
        ResourceSettings (const ResourceSettings&);

        QString getTransformedVariablePath(QString _path);

        // QSetting instance to find and add setting
        QSettings m_settings;

        QString m_deploymentUrl;
        QString m_appName;

        QString m_defaultLocale;
        QString m_shortcutName;
        bool m_runAtStart;

        QString m_defaultInstallationPath;
        bool m_changeDataLocationAllowed;
};

#endif
