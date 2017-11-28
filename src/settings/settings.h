#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QString>
#include <QVariant>
#include <QObject>
#include <QMutex>

static QString GROUP_PROXY = "PROXY";
static QString GROUP_LANGUAGE = "LANGUAGE";
static QString GROUP_SHORTCUT = "SHORTCUT";
static QString GROUP_CLASSPATH = "CLASSPATH";
static QString GROUP_DATA = "DATA";
static QString GROUP_SERVER = "SERVER";

static QString S_PROXY_USING = "isProxyUsing";
static QString S_PROXY_AUTO = "proxyAuto";
static QString S_PROXY_URL = "proxyURL";
static QString S_PROXY_PORT = "proxyPort";
static QString S_PROXY_AUTHENTICATION = "proxyAuthentication";
static QString S_PROXY_LOGIN = "proxyLogin";
static QString S_PROXY_PASSWORD = "proxyPassword";

static QString S_LANGUAGE_LANGUAGE = "language";

static QString S_SHORTCUT_NAME = "shortcutName";
static QString S_SHORTCUT_ALL_USER = "shortcutAllUser";

static QString S_CLASSPATH_EXTENSION = "classpathExtension";

static QString S_DATA_LOCATION = "dataLocation";

static QString S_SERVER_URL = "serverURL";

enum Language { English, French };

/**
 * \class Settings
 *
 * @brief  This class use QSettings of QT for mange settings in application.
 * Data settings are stored defferently depending on the OS :
 *  - register on windows
 *  - system preference on mac OS
 *  - text file in Unix
 */
class Settings
{
    public:
        static Settings& Instance();

        void initSettings(QString installPath);

        void writeSettings();

        void readSettings();

        bool getIsProxyUsing() const;
        void setIsProxyUsing(bool isProxyUsing);

        bool getProxyAuto() const;
        void setProxyAuto(bool proxyAuto);

        QString getProxyURL() const;
        void setProxyURL(const QString &proxyURL);

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

        static Settings m_instance;
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
        bool m_isProxyUsing;
        bool m_proxyAuto;
        QString m_proxyURL;
        int m_proxyPort;
        bool m_proxyAuthentification;
        QString m_proxyLogin;
        QString m_proxyPassword;

        // language
        Language m_language;

        // shortcut
        QString m_shortcutName;
        bool m_shortcutAllUser;

        // classpath downloader
        QString m_classpathExtension;

        // data
        QString m_dataLocation;

        // server
        QString m_serverURL;
};

#endif // SETTINGS_H