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
        static const QString ShortcutOfflineName;
        static const QString ShortcutOffline;
        static const QString ShortcutOfflineArgs;

        static const QString RunAtStart;

        static const QString DefaultInstallationPath;

        static const QString ChangeDataLocationAllowed;

        static const QString InsetColor;
        static const QString PanelBackgroundColor;
        static const QString ButtonHoverBackgroundColor;
        static const QString ButtonBackgroundColor;
        static const QString DefaultTextColor;
        static const QString GrayTextColor;
        static const QString DisabledColor;
        static const QString WindowBorderWidth;

        ResourceSettings(const QString &_appPath);
        virtual ~ResourceSettings();

        void readSettings();

        QString getWindowBorderWidth() const;
        QString getDisabledColor() const;
        QString getGrayTextColor() const;
        QString getDefaultTextColor() const;
        QString getButtonBackgroundColor() const;
        QString getButtonHoverBackgroundColor() const;
        QString getPanelBackgroundColor() const;
        QString getInsetColor() const;
        bool isChangeDataLocationAllowed() const;
        QString getDefaultInstallationPath() const;
        bool isShortcutOffline() const;
        QString getShortcutOfflineName() const;
        QString getShortcutName() const;
        QString getDefaultLocale() const;
        QString getAppName() const;
        QString getDeploymentUrl() const;
        QString getShortcutOfflineArgs() const;
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
        bool m_shortcutOffline;
        QString m_shortcutName;
        QString m_shortcutOfflineName;
        QString m_shortcutOfflineArgs;
        bool m_runAtStart;

        QString m_insetColor;
        QString m_panelBackgroundColor;
        QString m_buttonHoverBackgroundColor;
        QString m_buttonBackgroundColor;
        QString m_defaultTextColor;
        QString m_grayTextColor;
        QString m_disabledColor;
        QString m_windowBorderWidth;

        QString m_defaultInstallationPath;
        bool m_changeDataLocationAllowed;
};

#endif
