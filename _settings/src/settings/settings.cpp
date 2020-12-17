#include "settings/settings.h"
#include "log/logger.h"
#include "utils/crypt/cryptmanager.h"
#include "lang/languagemanager.h"
#include "secret/keymanager.h"

#include <QMutexLocker>
#include <QSettings>

const QString Settings::GroupProxy("proxy");
const QString Settings::GroupUninst("uninst");
const QString Settings::GroupConfig("config");

const QString Settings::AppName("app_name");
const QString Settings::UpdaterVersion("updater_version");
const QString Settings::JavaVersion("java_version");
const QString Settings::DataLocation("data_location");
const QString Settings::DeploymentUrl("deployment_url");
const QString Settings::Locale("locale");
const QString Settings::Style("style");

const QString Settings::ProxyUsed("proxy_use");
const QString Settings::ProxyHostname("proxy_hostname");
const QString Settings::ProxyPort("proxy_port");
const QString Settings::ProxyLogin("proxy_login");
const QString Settings::ProxyPassword("proxy_password");

const QString Settings::ShortcutName("shortcut_name");
const QString Settings::RunAtStart("run_at_start");

Settings * Settings::sm_instance = nullptr;
QMutex Settings::sm_instanceMutex;
QMutex Settings::sm_settingsMutex;

Settings::Settings() :
    m_settings(nullptr),
    m_appName("Application"),
    m_updaterVersion(""),
    m_javaVersion(""),
    m_dataLocation(""),
    m_deploymentUrl(""),
    m_locale(""),
    m_style(""),
    m_proxyUsed(false),
    m_proxyHostname(""),
    m_proxyPort(0),
    m_proxyLogin(""),
    m_proxyPassword(""),
    m_shortcutName("Application"),
    m_runAtStart(false)

{
    L_INFO("Initialise Setting singleton instance");
}

Settings::~Settings()
{
    delete m_settings;
}

Settings * Settings::getInstance()
{
    if (Settings::sm_instance == nullptr) {
        Settings::sm_instanceMutex.lock();
        if (Settings::sm_instance == nullptr) {
            Settings::sm_instance = new Settings();
        }
        Settings::sm_instanceMutex.unlock();
    }
    return Settings::sm_instance;
}

void Settings::kill()
{
    if (Settings::sm_instance) {
        Settings::sm_instanceMutex.lock();
        if (Settings::sm_instance) {
            delete Settings::sm_instance;
            Settings::sm_instance = nullptr;
        }
        Settings::sm_instanceMutex.unlock();
    }
}

void Settings::initSettings(QFile &_settingsFile)
{
    if (m_settings == nullptr) {
        m_settings = new QSettings(_settingsFile.fileName(), QSettings::IniFormat);
        m_settings->setFallbacksEnabled(false);
    }
}

void Settings::putSetting(QString _key, QVariant _value)
{
    m_settings->setValue(_key, _value);
}

QVariant Settings::getSetting(QString _key)
{
    return m_settings->value(_key);
}

QVariant Settings::getSetting(QString _key, QVariant _defaultValue)
{
    return m_settings->value(_key, _defaultValue);
}

void Settings::removeSetting(QString _key)
{
    m_settings->remove(_key);
}

bool Settings::isWritable()
{
    return m_settings->isWritable();
}

bool Settings::writeSettings()
{
    L_INFO("Starting to write all settings");
    QMutexLocker locker(&sm_settingsMutex);

    m_settings->beginGroup(GroupProxy);
    putSetting(ProxyUsed, m_proxyUsed);
    putSetting(ProxyHostname, m_proxyHostname);
    putSetting(ProxyPort, m_proxyPort);
    putSetting(ProxyLogin, m_proxyLogin);
    putSetting(ProxyPassword, m_proxyPassword);
    m_settings->endGroup();

    m_settings->beginGroup(GroupUninst);
    putSetting(ShortcutName, m_shortcutName);
    putSetting(RunAtStart, m_runAtStart);
    m_settings->endGroup();

    m_settings->beginGroup(GroupConfig);
    putSetting(AppName, m_appName);
    putSetting(Locale, m_locale);
    putSetting(Style, m_style);
    putSetting(UpdaterVersion, m_updaterVersion);
    putSetting(JavaVersion, m_javaVersion);
    putSetting(DataLocation, m_dataLocation);
    putSetting(DeploymentUrl, m_deploymentUrl);
    m_settings->endGroup();

    L_INFO("End to write all settings");

    // write immediately in .ini file
    m_settings->sync();
    return (m_settings->status() == QSettings::NoError);
}

void Settings::readSettings()
{
    L_INFO("Starting to read all settings");
    QMutexLocker locker(&sm_settingsMutex);

    m_settings->beginGroup(GroupProxy);
    m_proxyUsed = getSetting(ProxyUsed, m_proxyUsed).toBool();
    m_proxyHostname = getSetting(ProxyHostname, "").toString();
    m_proxyPort = getSetting(ProxyPort, 0).toInt();
    m_proxyLogin = getSetting(ProxyLogin, "").toString();
    m_proxyPassword = getSetting(ProxyPassword, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupUninst);
    m_shortcutName = getSetting(ShortcutName, m_shortcutName).toString();
    m_runAtStart = getSetting(RunAtStart, m_runAtStart).toBool();
    m_settings->endGroup();

    m_settings->beginGroup(GroupConfig);
    m_appName = getSetting(AppName, m_appName).toString();
    m_locale = getSetting(Locale, "").toString();
    m_style = getSetting(Style, "default").toString();
    m_updaterVersion = getSetting(UpdaterVersion, m_updaterVersion).toString();
    m_javaVersion = getSetting(JavaVersion, m_javaVersion).toString();
    m_dataLocation = getSetting(DataLocation, m_dataLocation).toString();
    m_deploymentUrl = getSetting(DeploymentUrl, "").toString();
    m_settings->endGroup();

    L_INFO("End to read all settings");
}

QString Settings::paramListString()
{
    QString s = "app_name = " + m_appName + "\n";

    s += "updater_version = " + m_updaterVersion + "\n";
    s += "java_version = " + m_javaVersion + "\n";
    s += "proxy_use = " + QString::number(m_proxyUsed) + "\n";
    s += "proxy_hostname = " + m_proxyHostname + "\n";
    s += "proxy_port = " + QString::number(m_proxyPort) + "\n";
    s += "proxy_login = " + m_proxyLogin + "\n";
    s += "proxy_password (encrypted) = " + m_proxyPassword + "\n";
    s += "locale = " + m_locale + "\n";
    s += "style = " + m_style + "\n";
    s += "shortcut_name = " + m_shortcutName + "\n";
    s += "data_location = " + m_dataLocation + "\n";
    s += "deployment_url = " + m_deploymentUrl + "\n";
    s += "run_at_start = " + QString::number(m_runAtStart) + "\n";

    return s;
}

QString Settings::getUpdaterVersion() const
{
    return m_updaterVersion;
}

void Settings::setUpdaterVersion(const QString &updaterVersion)
{
    m_updaterVersion = updaterVersion;
}

QString Settings::getJavaVersion() const
{
    return m_javaVersion;
}

void Settings::setJavaVersion(const QString &_javaVersion)
{
    m_javaVersion = _javaVersion;
}

QString Settings::getAppName() const
{
    return m_appName;
}

void Settings::setAppName(const QString &appName)
{
    m_appName = appName;
}

bool Settings::isRunAtStart() const
{
    return m_runAtStart;
}

void Settings::setRunAtStart(const bool &runAtStart)
{
    m_runAtStart = runAtStart;
}

QString Settings::getDeploymentUrl() const
{
    return m_deploymentUrl;
}

void Settings::setDeploymentUrl(const QString &deploymentUrl)
{
    m_deploymentUrl = deploymentUrl;
}

QString Settings::getDataLocation() const
{
    return m_dataLocation;
}

void Settings::setDataLocation(const QString &dataLocation)
{
    m_dataLocation = dataLocation;
}

QString Settings::getShortcutName() const
{
    return m_shortcutName;
}

void Settings::setShortcutName(const QString &shortcutName)
{
    m_shortcutName = shortcutName;
}

QString Settings::getLocale() const
{
    return m_locale;
}

void Settings::setLocale(const QString &_locale)
{
    m_locale = _locale;
}

QString Settings::getStyle() const
{
    return m_style;
}

void Settings::setStyle(const QString &_style)
{
    m_style = _style;
}

QString Settings::getProxyPassword() const
{
    const QString key = KeyManager::readPasswordEncryptionKey();

    return CryptManager::decrypt(key, m_proxyPassword);
}

void Settings::setProxyPassword(const QString &proxyPassword)
{
    const QString key = KeyManager::readPasswordEncryptionKey();

    m_proxyPassword = CryptManager::encrypt(key, proxyPassword);
}

QString Settings::getProxyLogin() const
{
    return m_proxyLogin;
}

void Settings::setProxyLogin(const QString &proxyLogin)
{
    m_proxyLogin = proxyLogin;
}

int Settings::getProxyPort() const
{
    return m_proxyPort;
}

void Settings::setProxyPort(int proxyPort)
{
    m_proxyPort = proxyPort;
}

QString Settings::getProxyHostname() const
{
    return m_proxyHostname;
}

void Settings::setProxyHostname(const QString &proxyHostname)
{
    m_proxyHostname = proxyHostname;
}

bool Settings::isProxyUsed() const
{
    return m_proxyUsed;
}

void Settings::setProxyUsed(bool proxyUsed)
{
    m_proxyUsed = proxyUsed;
}
