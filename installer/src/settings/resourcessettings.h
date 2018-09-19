#ifndef SETTINGS__RESOURCESSETTINGS_H
#define SETTINGS__RESOURCESSETTINGS_H

#include <QString>
#include <QSettings>

class ResourcesSettings
{
    public:

        static const QString DeploymentUrl;
        static const QString AppName;
        static const QString Lang;

        static const QString ShortcutName;
        static const QString ShortcutOfflineName;
        static const QString ShortcutOnline;
        static const QString ShortcutOffline;
        static const QString ShortcutOfflineArgs;

        static const QString RunAtStart;

        static const QString DefaultInstallationPath;

        static const QString DefaultSimpleInstallDataPath;
        static const QString DefaultCustomInstallDataPath;
        static const QString ChangeDataLocationAllowed;

        static const QString InsetColor;
        static const QString PanelBackgroundColor;
        static const QString ButtonHoverBackgroundColor;
        static const QString ButtonBackgroundColor;
        static const QString DefaultTextColor;
        static const QString GrayTextColor;
        static const QString DisabledColor;
        static const QString WindowBorderWidth;

        ResourcesSettings(const QString &_appPath);
        virtual ~ResourcesSettings();

        void readSettings();
        void writeAppSettings();

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
        bool isShortcutOnline() const;
        QString getShortcutOfflineName() const;
        QString getShortcutName() const;
        QString getLang() const;
        QString getAppName() const;
        QString getDeploymentUrl() const;
        QString getDefaultSimpleInstallDataPath() const;
        QString getDefaultCustomInstallDataPath() const;
        QString getShortcutOfflineArgs() const;
        bool isRunAtStart() const;

    private:

        void operator=(const ResourcesSettings&);
        ResourcesSettings (const ResourcesSettings&);

        QString getTransformedVariablePath(QString _path);

        // QSetting instance to find and add setting
        QSettings m_settings;

        QString m_deploymentUrl;
        QString m_appName;

        QString m_lang;

        bool m_shortcutOnline;
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
        QString m_defaultSimpleInstallDataPath;
        QString m_defaultCustomInstallDataPath;
        bool m_changeDataLocationAllowed;
};

#endif
