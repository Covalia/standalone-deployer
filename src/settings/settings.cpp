#include "settings.h"
#include "../log/simpleQtLogger.h"

#include <QMutexLocker>

Settings Settings::m_instance = Settings();
QMutex Settings::sm_instanceMutex;
QMutex Settings::sm_settingsMutex;

Settings::Settings()
{
    L_INFO("Initialise Setting singleton instance");

    m_proxyUse = false;
    m_proxyAuto = false;
    m_proxyManual = false;
    m_proxyURL = "";
    m_proxyPort = -1;
    m_proxyAuthentification = false;
    m_proxyLogin = "";
    m_proxyPassword = "";

    m_language = Language::English;

    m_shortcutName = "";
    m_shortcutOffline = false;
    m_shortcutOnline = false;
    m_shortcutAllUser = false;

    m_classpathExtension = "";

    m_installLocation = "";
    m_dataLocation = "";

    m_serverURL = "";

    m_runAtStart = true;
}

Settings::~Settings()
{
    delete m_settings;
}

Settings& Settings::Instance()
{
    return m_instance;
}

void Settings::initSettings(QString installPath)
{
    m_settings = new QSettings(installPath, QSettings::IniFormat);
}

void Settings::putSetting(QString key, QVariant value)
{
    m_settings->setValue(key, value);
}

QVariant Settings::getSetting(QString key)
{
    return m_settings->value(key);
}

QVariant Settings::getSetting(QString key, QVariant defaultValue)
{
    return m_settings->value(key, defaultValue);
}

void Settings::removeSetting(QString key)
{
    m_settings->remove(key);
}


void Settings::writeSettings()
{
    L_INFO("Starting to write all settings");
    QMutexLocker locker(&sm_settingsMutex);

    m_settings->beginGroup(GROUP_PROXY);
    putSetting(S_PROXY_USE, m_proxyUse);
    putSetting(S_PROXY_AUTO, m_proxyAuto);
    putSetting(S_PROXY_MANUAL, m_proxyManual);
    putSetting(S_PROXY_URL, m_proxyURL);
    putSetting(S_PROXY_PORT, m_proxyPort);
    putSetting(S_PROXY_AUTHENTICATION, m_proxyAuthentification);
    putSetting(S_PROXY_LOGIN, m_proxyLogin);
    putSetting(S_PROXY_PASSWORD, m_proxyPassword);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_LANGUAGE);
    putSetting(S_LANGUAGE_LANGUAGE, m_language);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_SHORTCUT);
    putSetting(S_SHORTCUT_NAME, m_shortcutName);
    putSetting(S_SHORTCUT_OFFLINE, m_shortcutOffline);
    putSetting(S_SHORTCUT_ONLINE, m_shortcutOnline);
    putSetting(S_SHORTCUT_ALL_USER, m_shortcutAllUser);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_CLASSPATH);
    putSetting(S_CLASSPATH_EXTENSION, m_classpathExtension);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_DATA);
    putSetting(S_DATA_LOCATION, m_dataLocation);
    putSetting(S_INSTALL_LOCATION, m_installLocation);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_SERVER);
    putSetting(S_SERVER_URL, m_serverURL);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_START);
    putSetting(S_SERVER_URL, m_runAtStart);
    m_settings->endGroup();
} // Settings::writeSettings

void Settings::readSettings()
{
    L_INFO("Starting to read all settings");
    QMutexLocker locker(&sm_settingsMutex);

    m_settings->beginGroup(GROUP_PROXY);
    m_proxyUse = getSetting(S_PROXY_USE, false).toBool();
    m_proxyAuto = getSetting(S_PROXY_AUTO, false).toBool();
    m_proxyManual = getSetting(S_PROXY_MANUAL, false).toBool();
    m_proxyURL = getSetting(S_PROXY_URL, "").toString();
    m_proxyPort = getSetting(S_PROXY_PORT, 0).toInt();
    m_proxyAuthentification = getSetting(S_PROXY_AUTHENTICATION, false).toBool();
    m_proxyLogin = getSetting(S_PROXY_LOGIN, "").toString();
    m_proxyPassword = getSetting(S_PROXY_PASSWORD, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_LANGUAGE);
    m_language = Language(getSetting(S_LANGUAGE_LANGUAGE, 0).toInt());
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_SHORTCUT);
    m_shortcutName = getSetting(S_SHORTCUT_NAME, "").toString();
    m_shortcutOffline = getSetting(S_SHORTCUT_OFFLINE, false).toBool();
    m_shortcutOnline = getSetting(S_SHORTCUT_ONLINE, false).toBool();
    m_shortcutAllUser = getSetting(S_SHORTCUT_ALL_USER, false).toBool();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_CLASSPATH);
    m_classpathExtension = getSetting(S_CLASSPATH_EXTENSION, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_DATA);
    m_dataLocation = getSetting(S_DATA_LOCATION, "").toString();
    m_installLocation = getSetting(S_INSTALL_LOCATION, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_SERVER);
    m_serverURL = getSetting(S_SERVER_URL, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_START);
    m_runAtStart = getSetting(S_RUN_AT_START, true).toBool();
    m_settings->endGroup();
} // Settings::readSettings

bool Settings::getRunAtStart() const
{
    return m_runAtStart;
}

void Settings::setRunAtStart(const bool &runAtStart)
{
    m_runAtStart = runAtStart;
}

QString Settings::getServerURL() const
{
    return m_serverURL;
}

void Settings::setServerURL(const QString &serverURL)
{
    m_serverURL = serverURL;
}

QString Settings::getDataLocation() const
{
    return m_dataLocation;
}

QString Settings::getInstallLocation() const
{
    return m_installLocation;
}

void Settings::setInstallLocation(const QString &installLocation)
{
    m_installLocation = installLocation;
}

void Settings::setDataLocation(const QString &dataLocation)
{
    m_dataLocation = dataLocation;
}

QString Settings::getClasspathExtension() const
{
    return m_classpathExtension;
}

void Settings::setClasspathExtension(const QString &classpathExtension)
{
    m_classpathExtension = classpathExtension;
}

bool Settings::getShortcutAllUser() const
{
    return m_shortcutAllUser;
}

void Settings::setShortcutAllUser(bool shortcutAllUser)
{
    m_shortcutAllUser = shortcutAllUser;
}

bool Settings::getShortcutOnline() const
{
    return m_shortcutOnline;
}

void Settings::setShortcutOnline(bool shortcutOnline)
{
    m_shortcutOnline = shortcutOnline;
}

bool Settings::getShortcutOffline() const
{
    return m_shortcutOffline;
}

void Settings::setShortcutOffline(bool shortcutOffline)
{
    m_shortcutOffline = shortcutOffline;
}

QString Settings::getShortcutName() const
{
    return m_shortcutName;
}

void Settings::setShortcutName(const QString &shortcutName)
{
    m_shortcutName = shortcutName;
}

Language Settings::getLanguage() const
{
    return m_language;
}

void Settings::setLanguage(const Language &language)
{
    m_language = language;
}

QString Settings::getProxyPassword() const
{
    return m_proxyPassword;
}

void Settings::setProxyPassword(const QString &proxyPassword)
{
    m_proxyPassword = proxyPassword;
}

QString Settings::getProxyLogin() const
{
    return m_proxyLogin;
}

void Settings::setProxyLogin(const QString &proxyLogin)
{
    m_proxyLogin = proxyLogin;
}

bool Settings::getProxyAuthentification() const
{
    return m_proxyAuthentification;
}

void Settings::setProxyAuthentification(bool proxyAuthentification)
{
    m_proxyAuthentification = proxyAuthentification;
}

int Settings::getProxyPort() const
{
    return m_proxyPort;
}

void Settings::setProxyPort(int proxyPort)
{
    m_proxyPort = proxyPort;
}

QString Settings::getProxyURL() const
{
    return m_proxyURL;
}

void Settings::setProxyURL(const QString &proxyURL)
{
    m_proxyURL = proxyURL;
}

bool Settings::getProxyManual() const
{
    return m_proxyManual;
}

void Settings::setProxyManual(bool proxyManual)
{
    m_proxyManual = proxyManual;
}

bool Settings::getProxyAuto() const
{
    return m_proxyAuto;
}

void Settings::setProxyAuto(bool proxyAuto)
{
    m_proxyAuto = proxyAuto;
}

bool Settings::getProxyUse() const
{
    return m_proxyUse;
}

void Settings::setProxyUse(bool proxyUse)
{
    m_proxyUse = proxyUse;
}
