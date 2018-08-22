#ifndef SETTINGS__RESOURCESSETTINGS_H
#define SETTINGS__RESOURCESSETTINGS_H

#include <QString>
#include <QSettings>
#include <QMutex>

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

        static ResourcesSettings * getInstance();
        static void kill();

        void initSettings(QString _appPath);

        void readSettings();

        void writeAppSettings();

        QString getWindowBorderWidth() const;
        void setWindowBorderWidth(const QString &windowBorderWidth);

        QString getDisabledColor() const;
        void setDisabledColor(const QString &disabledColor);

        QString getGrayTextColor() const;
        void setGrayTextColor(const QString &grayTextColor);

        QString getDefaultTextColor() const;
        void setDefaultTextColor(const QString &defaultTextColor);

        QString getButtonBackgroundColor() const;
        void setButtonBackgroundColor(const QString &buttonBackgroundColor);

        QString getButtonHoverBackgroundColor() const;
        void setButtonHoverBackgroundColor(const QString &buttonHoverBackgroundColor);

        QString getPanelBackgroundColor() const;
        void setPanelBackgroundColor(const QString &panelBackgroundColor);

        QString getInsetColor() const;
        void setInsetColor(const QString &insetColor);

        bool isChangeDataLocationAllowed() const;
        void setChangeDataLocationAllowed(bool changeDataLocationAllowed);

        QString getDefaultInstallationPath() const;
        void setDefaultInstallationPath(const QString &defaultInstallationPath);

        bool isShortcutOffline() const;
        void setShortcutOffline(bool shortcutOffline);

        bool isShortcutOnline() const;
        void setShortcutOnline(bool shortcutOnline);

        QString getShortcutOfflineName() const;
        void setShortcutOfflineName(const QString &shortcutOfflineName);

        QString getShortcutName() const;
        void setShortcutName(const QString &shortcutName);

        QString getLang() const;
        void setLang(const QString &lang);

        QString getAppName() const;
        void setAppName(const QString &appName);

        QString getDeploymentUrl() const;
        void setDeploymentUrl(const QString &deploymentUrl);

        QString getDefaultSimpleInstallDataPath() const;
        void setDefaultSimpleInstallDataPath(const QString &defaultSimpleInstallDataPath);

        QString getDefaultCustomInstallDataPath() const;
        void setDefaultCustomInstallDataPath(const QString &defaultCustomInstallDataPath);

        QString getShortcutOfflineArgs() const;
        void setShortcutOfflineArgs(const QString &shortcutOfflineArgs);

        bool isRunAtStart() const;
        void setRunAtStart(bool runAtStart);

    private:
        /**
         * @brief Use for singleton
         */
        void operator=(const ResourcesSettings&)
        {
        }
        ResourcesSettings (const ResourcesSettings&)
        {
        }

        static ResourcesSettings * sm_instance;
        static QMutex sm_instanceMutex;
        static QMutex sm_settingsMutex;

        /**
         * @brief Constructor in singleton
         */
        ResourcesSettings();

        /**
         * @brief Destructor in singleton
         */
        virtual ~ResourcesSettings();

        QString getTransformedVariablePath(QString _path);

        /**
         * @brief QSetting instance for find and add setting
         */
        QSettings * m_settings;

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

#endif // SETTINGS__RESOURCESSETTINGS_H
