#ifndef SETTINGS_H
#define SETTINGS_H

#include "lang/languagemanager.h"

#include <QString>
#include <QVariant>
#include <QMutex>

class QSettings;

static QString GROUP_PROXY = "PROXY";
static QString GROUP_LANGUAGE = "LANGUAGE";
static QString GROUP_SHORTCUT = "SHORTCUT";
static QString GROUP_CLASSPATH = "CLASSPATH";
static QString GROUP_DATA = "DATA";
static QString GROUP_SERVER = "SERVER";
static QString GROUP_START = "START";

static QString S_PROXY_USE = "proxyUse";
static QString S_PROXY_AUTO = "proxyAuto";
static QString S_PROXY_MANUAL = "proxyManual";
static QString S_PROXY_HOSTNAME = "proxyHostname";
static QString S_PROXY_PORT = "proxyPort";
static QString S_PROXY_AUTHENTICATION = "proxyAuthentication";
static QString S_PROXY_LOGIN = "proxyLogin";
static QString S_PROXY_PASSWORD = "proxyPassword";

static QString S_LANGUAGE_LANGUAGE = "language";

static QString S_SHORTCUT_NAME = "shortcutName";
static QString S_SHORTCUT_OFFLINE = "shortcutOffline";
static QString S_SHORTCUT_ONLINE = "shortcutOnline";
static QString S_SHORTCUT_ALL_USER = "shortcutAllUser";

static QString S_CLASSPATH_EXTENSION = "classpathExtension";

static QString S_INSTALL_LOCATION = "installLocation";
static QString S_DATA_LOCATION = "dataLocation";


static QString S_SERVER_URL = "serverURL";

static QString S_RUN_AT_START = "runAtStart";



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

        void initSettings(QString installPath);

        void writeSettings();

        void readSettings();

        bool getProxyUse() const;
        void setProxyUse(bool proxyUse);

        bool getProxyAuto() const;
        void setProxyAuto(bool proxyAuto);

        bool getProxyManual() const;
        void setProxyManual(bool proxyManual);

        QString getProxyHostname() const;
        void setProxyHostname(const QString &proxyHostname);

        int getProxyPort() const;
        void setProxyPort(int proxyPort);

        bool getProxyAuthentification() const;
        void setProxyAuthentification(bool proxyAuthentification);

        QString getProxyLogin() const;
        void setProxyLogin(const QString &proxyLogin);

        QString getProxyPassword() const;
        void setProxyPassword(const QString &proxyPassword);

        Language getLanguage() const;
        void setLanguage(const Language &language);

        QString getShortcutName() const;
        void setShortcutName(const QString &shortcutName);

        bool getShortcutAllUser() const;
        void setShortcutAllUser(bool shortcutAllUser);

        QString getClasspathExtension() const;
        void setClasspathExtension(const QString &classpathExtension);

        QString getDataLocation() const;
        void setDataLocation(const QString &dataLocation);

        QString getServerURL() const;
        void setServerURL(const QString &serverURL);

        bool getRunAtStart() const;
        void setRunAtStart(const bool &runAtStart);

        QString getInstallLocation() const;
        void setInstallLocation(const QString &installLocation);

        bool getShortcutOffline() const;
        void setShortcutOffline(bool shortcutOffline);

        bool getShortcutOnline() const;
        void setShortcutOnline(bool shortcutOnline);

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
        void putSetting(QString key, QVariant value);

        /**
         * @brief Find setting value with key
         * @param key
         * @return
         */
        QVariant getSetting(QString key);

        /**
         * @brief Find setting value with key, and return default value if key not find
         * @param key
         * @param defaultValue
         * @return
         */
        QVariant getSetting(QString key, QVariant defaultValue);

        /**
         * @brief Remove a setting with key
         * @param key
         */
        void removeSetting(QString key);

        /**
         * @brief QSetting instance for find and add setting
         */
        QSettings * m_settings;

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
        QString m_shortcutName;
        bool m_shortcutOffline;
        bool m_shortcutOnline;
        bool m_shortcutAllUser;

        // classpath downloader
        QString m_classpathExtension;

        // data
        QString m_dataLocation;
        QString m_installLocation;

        // server
        QString m_serverURL;

        // start
        bool m_runAtStart;
};

#endif // SETTINGS_H
