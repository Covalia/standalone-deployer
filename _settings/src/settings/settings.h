#ifndef SETTINGS__SETTINGS_H
#define SETTINGS__SETTINGS_H

#include "lang/languagemanager.h"

#include <QString>
#include <QFile>
#include <QVariant>
#include <QMutex>

class QSettings;

static QString GROUP_INFO = "INFO";
static QString GROUP_PROXY = "PROXY";
static QString GROUP_LANGUAGE = "LANGUAGE";
static QString GROUP_SHORTCUT = "SHORTCUT";
static QString GROUP_DATA = "DATA";
static QString GROUP_SERVER = "SERVER";
static QString GROUP_START = "START";
static QString GROUP_THEME = "THEME";

static QString S_APPLICATION_NAME = "application_name";
static QString S_UPDATER_VERSION = "updater_version";
static QString S_JAVA_VERSION = "java_version";

static QString S_PROXY_USE = "proxyUse";
static QString S_PROXY_AUTO = "proxyAuto";
static QString S_PROXY_MANUAL = "proxyManual";
static QString S_PROXY_HOSTNAME = "proxyHostname";
static QString S_PROXY_PORT = "proxyPort";
static QString S_PROXY_AUTHENTICATION = "proxyAuthentication";
static QString S_PROXY_LOGIN = "proxyLogin";
static QString S_PROXY_PASSWORD = "proxyPassword";

static QString S_LANGUAGE_LANGUAGE = "language";

static QString S_SHORTCUT_ONLINE = "shortcutOnline";
static QString S_SHORTCUT_OFFLINE = "shortcutOffline";
static QString S_SHORTCUT_NAME = "shortcutName";
static QString S_SHORTCUT_OFFLINE_NAME = "shortcutOfflineName";
static QString S_SHORTCUT_OFFLINE_ARGS = "shortcutOfflineArgs";
static QString S_SHORTCUT_ALL_USER = "shortcutAllUser";

static QString S_INSTALL_LOCATION = "installLocation";
static QString S_DATA_LOCATION = "dataLocation";

static QString S_DEPLOYMENT_URL = "deployment_url";

static QString S_RUN_AT_START = "runAtStart";

static QString S_COLOR_PANEL_BACKGROUND_BORDER = "color_panel_background_border";
static QString S_COLOR_PANEL_BACKGROUND = "color_panel_background";
static QString S_COLOR_BUTTON_BACKGROUND_OVER = "color_button_background_over";
static QString S_COLOR_BUTTON_BACKGROUND = "color_button_background";
static QString S_COLOR_TEXT_ON_BACKGROUND = "color_text_on_background";
static QString S_COLOR_TEXT_GRAY = "color_text_gray";
static QString S_COLOR_DISABLED = "color_disabled";
static QString S_BORDER_WINDOW = "border_window";



/**
 * \class Settings
 *
 * @brief  This class use QSettings of QT for manage settings in application.
 */
class Settings
{
    public:
        static Settings *getInstance();
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

        Language getLanguage() const;
        void setLanguage(const Language &language);

        QString getShortcutName() const;
        void setShortcutName(const QString &shortcutName);

        bool isShortcutAllUser() const;
        void setShortcutAllUser(bool shortcutAllUser);

        QString getDataLocation() const;
        void setDataLocation(const QString &dataLocation);

        QString getDeploymentUrl() const;
        void setDeploymentUrl(const QString &deploymentUrl);

        bool isRunAtStart() const;
        void setRunAtStart(const bool &runAtStart);

        QString getInstallLocation() const;
        void setInstallLocation(const QString &installLocation);

        bool isShortcutOffline() const;
        void setShortcutOffline(bool shortcutOffline);

        bool isShortcutOnline() const;
        void setShortcutOnline(bool shortcutOnline);

        QString getApplicationName() const;
        void setApplicationName(const QString &applicationName);

        QString getUpdaterVersion() const;
        void setUpdaterVersion(const QString &updaterVersion);

        QString getJavaVersion() const;
        void setJavaVersion(const QString &_javaVersion);

        QString getShortcutOfflineName() const;
        void setShortcutOfflineName(const QString &shortcutOfflineName);

        QString getShortcutOfflineArgs() const;
        void setShortcutOfflineArgs(const QString &shortcutOfflineArgs);

        QString getColorPanelBackgroundBorder() const;
        void setColorPanelBackgroundBorder(const QString &colorPanelBackgroundBorder);

        QString getColorPanelBackground() const;
        void setColorPanelBackground(const QString &colorPanelBackground);

        QString getColorButtonBackgroundOver() const;
        void setColorButtonBackgroundOver(const QString &colorButtonBackgroundOver);

        QString getColorButtonBackground() const;
        void setColorButtonBackground(const QString &colorButtonBackground);

        QString getColorTextOnBackground() const;
        void setColorTextOnBackground(const QString &colorTextOnBackground);

        QString getColorTextGray() const;
        void setColorTextGray(const QString &colorTextGray);

        QString getColorDisabled() const;
        void setColorDisabled(const QString &colorDisabled);

        QString getBorderWindow() const;
        void setBorderWindow(const QString &borderWindow);

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

        // info
        QString m_applicationName;
        QString m_updaterVersion;
        QString m_javaVersion;

        // proxy
        bool m_proxyUse;
        bool m_proxyAuto;
        bool m_proxyManual;
        QString m_proxyHostname;
        int m_proxyPort;
        bool m_proxyAuthentification;
        QString m_proxyLogin;
        QString m_proxyPassword;

        // language
        Language m_language;

        // shortcut
        bool m_shortcutOnline;
        bool m_shortcutOffline;
        QString m_shortcutName;
        QString m_shortcutOfflineName;
        QString m_shortcutOfflineArgs;
        bool m_shortcutAllUser;

        // data
        QString m_dataLocation;
        QString m_installLocation;

        // server
        QString m_deploymentUrl;

        // start
        bool m_runAtStart;

        // theme
        QString m_colorPanelBackgroundBorder;
        QString m_colorPanelBackground;
        QString m_colorButtonBackgroundOver;
        QString m_colorButtonBackground;
        QString m_colorTextOnBackground;
        QString m_colorTextGray;
        QString m_colorDisabled;
        QString m_borderWindow;
};

#endif // SETTINGS__SETTINGS_H
