#ifndef SETTINGS__SETTINGS_H
#define SETTINGS__SETTINGS_H

#include "lang/languagemanager.h"

#include <QString>
#include <QFile>
#include <QVariant>
#include <QMutex>

class QSettings;

/**
 * \class Settings
 *
 * @brief  This class use QSettings of QT for manage settings in application.
 */
class Settings
{
    public:

        static const QString GroupInfo;
        static const QString GroupProxy;
        static const QString GroupLang;
        static const QString GroupShortcut;
        static const QString GroupData;
        static const QString GroupServer;
        static const QString GroupStart;
        static const QString GroupTheme;

        static const QString AppName;
        static const QString UpdaterVersion;
        static const QString JavaVersion;

        static const QString ProxyUse;
        static const QString ProxyAuto;
        static const QString ProxyManual;
        static const QString ProxyHostname;
        static const QString ProxyPort;
        static const QString ProxyAuthentication;
        static const QString ProxyLogin;
        static const QString ProxyPassword;

        static const QString Lang;

        static const QString ShortcutOnline;
        static const QString ShortcutOffline;
        static const QString ShortcutName;
        static const QString ShortcutOfflineName;
        static const QString ShortcutOfflineArgs;
        static const QString ShortcutForAllUsers;

        static const QString DataLocation;

        static const QString DeploymentUrl;

        static const QString RunAtStart;

        static const QString InsetColor;
        static const QString PanelBackgroundColor;
        static const QString ButtonHoverBackgroundColor;
        static const QString ButtonBackgroundColor;
        static const QString ColorTextOnBackground;
        static const QString GrayTextColor;
        static const QString DisabledColor;
        static const QString WindowBorderWidth;

        static Settings * getInstance();
        static void kill();

        void initSettings(QFile &_settingsFile);

        bool isWritable();

        bool writeSettings();

        void readSettings();

        QString paramListString();

        bool isProxyUse() const;
        void setProxyUse(bool proxyUse);

        bool isProxyAuto() const;
        void setProxyAuto(bool proxyAuto);

        bool isProxyManual() const;
        void setProxyManual(bool proxyManual);

        QString getProxyHostname() const;
        void setProxyHostname(const QString &proxyHostname);

        int getProxyPort() const;
        void setProxyPort(int proxyPort);

        bool isProxyAuthentification() const;
        void setProxyAuthentification(bool proxyAuthentification);

        QString getProxyLogin() const;
        void setProxyLogin(const QString &proxyLogin);

        QString getProxyPassword() const;
        void setProxyPassword(const QString &proxyPassword);

        Language getLang() const;
        void setLang(const Language &lang);

        QString getShortcutName() const;
        void setShortcutName(const QString &shortcutName);

        bool isShortcutForAllUsers() const;
        void setShortcutForAllUsers(bool shortcutForAllUsers);

        QString getDataLocation() const;
        void setDataLocation(const QString &dataLocation);

        QString getDeploymentUrl() const;
        void setDeploymentUrl(const QString &deploymentUrl);

        bool isRunAtStart() const;
        void setRunAtStart(const bool &runAtStart);

        bool isShortcutOffline() const;
        void setShortcutOffline(bool shortcutOffline);

        bool isShortcutOnline() const;
        void setShortcutOnline(bool shortcutOnline);

        QString getAppName() const;
        void setAppName(const QString &appName);

        QString getUpdaterVersion() const;
        void setUpdaterVersion(const QString &updaterVersion);

        QString getJavaVersion() const;
        void setJavaVersion(const QString &_javaVersion);

        QString getShortcutOfflineName() const;
        void setShortcutOfflineName(const QString &shortcutOfflineName);

        QString getShortcutOfflineArgs() const;
        void setShortcutOfflineArgs(const QString &shortcutOfflineArgs);

        QString getInsetColor() const;
        void setInsetColor(const QString &insetColor);

        QString getPanelBackgroundColor() const;
        void setPanelBackgroundColor(const QString &colorPanelBackground);

        QString getButtonHoverBackgroundColor() const;
        void setButtonHoverBackgroundColor(const QString &buttonHoverBackgroundColor);

        QString getButtonBackgroundColor() const;
        void setButtonBackgroundColor(const QString &buttonBackgroundColor);

        QString getDefaultTextColor() const;
        void setDefaultTextColor(const QString &defaultTextColor);

        QString getGrayTextColor() const;
        void setGrayTextColor(const QString &grayTextColor);

        QString getDisabledColor() const;
        void setDisabledColor(const QString &disabledColor);

        QString getWindowBorderWidth() const;
        void setWindowBorderWidth(const QString &windowBorderWidth);

    private:

        /**
         * @brief Use for singleton
         */
        void operator=(const Settings&)
        {
        }
        Settings (const Settings&)
        {
        }

        static Settings * sm_instance;
        static QMutex sm_instanceMutex;
        static QMutex sm_settingsMutex;

        /**
         * @brief Constructor in singleton
         */
        Settings();

        /**
         * @brief Destructor in singleton
         */
        virtual ~Settings();

        /**
         * @brief Add value in settings with key.
         * @param key
         * @param value
         */
        void putSetting(QString _key, QVariant _value);

        /**
         * @brief Find setting value with key
         * @param key
         * @return
         */
        QVariant getSetting(QString _key);

        /**
         * @brief Find setting value with key, and return default value if key not find
         * @param key
         * @param defaultValue
         * @return
         */
        QVariant getSetting(QString _key, QVariant _defaultValue);

        /**
         * @brief Remove a setting with key
         * @param key
         */
        void removeSetting(QString _key);

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

        bool m_shortcutForAllUsers;

        QString m_dataLocation;

        QString m_updaterVersion;
        QString m_javaVersion;

        bool m_proxyUse;
        bool m_proxyAuto;
        bool m_proxyManual;
        QString m_proxyHostname;
        int m_proxyPort;
        bool m_proxyAuthentification;
        QString m_proxyLogin;
        QString m_proxyPassword;
};

#endif // SETTINGS__SETTINGS_H
