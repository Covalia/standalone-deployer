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
const QString Settings::GroupTheme("theme");

const QString Settings::AppName("app_name");
const QString Settings::UpdaterVersion("updater_version");
const QString Settings::JavaVersion("java_version");

const QString Settings::ProxyUse("proxy_use");
const QString Settings::ProxyHostname("proxy_hostname");
const QString Settings::ProxyPort("proxy_port");
const QString Settings::ProxyLogin("proxy_login");
const QString Settings::ProxyPassword("proxy_password");

const QString Settings::Lang("lang");

const QString Settings::ShortcutOnline("shortcut_online");
const QString Settings::ShortcutOffline("shortcut_offline");
const QString Settings::ShortcutName("shortcut_name");
const QString Settings::ShortcutOfflineName("shortcut_offline_name");
const QString Settings::ShortcutOfflineArgs("shortcut_offline_args");
const QString Settings::ShortcutForAllUsers("shortcut_for_all_users");

const QString Settings::DataLocation("data_location");

const QString Settings::DeploymentUrl("deployment_url");

const QString Settings::RunAtStart("run_at_start");

const QString Settings::InsetColor("inset_color");
const QString Settings::PanelBackgroundColor("panel_background_color");
const QString Settings::ButtonHoverBackgroundColor("button_hover_background_color");
const QString Settings::ButtonBackgroundColor("button_background_color");
const QString Settings::ColorTextOnBackground("default_text_color");
const QString Settings::GrayTextColor("gray_text_color");
const QString Settings::DisabledColor("disabled_color");
const QString Settings::WindowBorderWidth("window_border_width");

Settings * Settings::sm_instance = 0;
QMutex Settings::sm_instanceMutex;
QMutex Settings::sm_settingsMutex;

Settings::Settings() :
    m_settings(0),
    m_deploymentUrl(""),
    m_appName("Application"),
    m_lang(LanguageManager::getLocaleFromLanguage(Language::English)),
    m_shortcutOnline(false),
    m_shortcutOffline(false),
    m_shortcutName("Application"),
    m_shortcutOfflineName("Application offline"),
    m_shortcutOfflineArgs("--offline=true"),
    m_runAtStart(false),
    m_insetColor("#364058"),
    m_panelBackgroundColor("#2d364c"),
    m_buttonHoverBackgroundColor("#2a7d7d"),
    m_buttonBackgroundColor("#339999"),
    m_defaultTextColor("#eff0f2"),
    m_grayTextColor("#9ea0a5"),
    m_disabledColor("#656976"),
    m_windowBorderWidth("0"),
    m_shortcutForAllUsers(false),
    m_dataLocation(""),
    m_updaterVersion(""),
    m_javaVersion(""),
    m_proxyUse(false),
    m_proxyHostname(""),
    m_proxyPort(0),
    m_proxyLogin(""),
    m_proxyPassword("")

{
    L_INFO("Initialise Setting singleton instance");
}

Settings::~Settings()
{
    delete m_settings;
}

Settings * Settings::getInstance()
{
    if (Settings::sm_instance == 0) {
        Settings::sm_instanceMutex.lock();
        if (Settings::sm_instance == 0) {
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
            Settings::sm_instance = 0;
        }
        Settings::sm_instanceMutex.unlock();
    }
}

void Settings::initSettings(QFile &_settingsFile)
{
    if (m_settings == 0) {
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
    putSetting(ProxyUse, m_proxyUse);
    putSetting(ProxyHostname, m_proxyHostname);
    putSetting(ProxyPort, m_proxyPort);
    putSetting(ProxyLogin, m_proxyLogin);
    putSetting(ProxyPassword, m_proxyPassword);
    m_settings->endGroup();

    m_settings->beginGroup(GroupUninst);
    putSetting(ShortcutOffline, m_shortcutOffline);
    putSetting(ShortcutOnline, m_shortcutOnline);
    putSetting(ShortcutName, m_shortcutName);
    putSetting(ShortcutOfflineName, m_shortcutOfflineName);
    putSetting(ShortcutOfflineArgs, m_shortcutOfflineArgs);
    putSetting(ShortcutForAllUsers, m_shortcutForAllUsers);
    putSetting(RunAtStart, m_runAtStart);
    m_settings->endGroup();

    m_settings->beginGroup(GroupConfig);
    putSetting(AppName, m_appName);
    putSetting(Lang, m_lang);
    putSetting(UpdaterVersion, m_updaterVersion);
    putSetting(JavaVersion, m_javaVersion);
    putSetting(DataLocation, m_dataLocation);
    putSetting(DeploymentUrl, m_deploymentUrl);
    m_settings->endGroup();

    m_settings->beginGroup(GroupTheme);
    putSetting(InsetColor, m_insetColor);
    putSetting(PanelBackgroundColor, m_panelBackgroundColor);
    putSetting(ButtonHoverBackgroundColor, m_buttonHoverBackgroundColor);
    putSetting(ButtonBackgroundColor, m_buttonBackgroundColor);
    putSetting(ColorTextOnBackground, m_defaultTextColor);
    putSetting(GrayTextColor, m_grayTextColor);
    putSetting(DisabledColor, m_disabledColor);
    putSetting(WindowBorderWidth, m_windowBorderWidth);
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
    m_proxyUse = getSetting(ProxyUse, m_proxyUse).toBool();
    m_proxyHostname = getSetting(ProxyHostname, "").toString();
    m_proxyPort = getSetting(ProxyPort, 0).toInt();
    m_proxyLogin = getSetting(ProxyLogin, "").toString();
    m_proxyPassword = getSetting(ProxyPassword, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupUninst);
    m_shortcutOffline = getSetting(ShortcutOffline, m_shortcutOffline).toBool();
    m_shortcutOnline = getSetting(ShortcutOnline, m_shortcutOnline).toBool();
    m_shortcutName = getSetting(ShortcutName, m_shortcutName).toString();
    m_shortcutOfflineName = getSetting(ShortcutOfflineName, m_shortcutOfflineName).toString();
    m_shortcutOfflineArgs = getSetting(ShortcutOfflineArgs, m_shortcutOfflineArgs).toString();
    m_shortcutForAllUsers = getSetting(ShortcutForAllUsers, m_shortcutForAllUsers).toBool();
    m_runAtStart = getSetting(RunAtStart, m_runAtStart).toBool();
    m_settings->endGroup();

    m_settings->beginGroup(GroupConfig);
    m_appName = getSetting(AppName, m_appName).toString();
    m_lang = getSetting(Lang, "").toString();
    m_updaterVersion = getSetting(UpdaterVersion, m_updaterVersion).toString();
    m_javaVersion = getSetting(JavaVersion, m_javaVersion).toString();
    m_dataLocation = getSetting(DataLocation, m_dataLocation).toString();
    m_deploymentUrl = getSetting(DeploymentUrl, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupTheme);
    m_insetColor = getSetting(InsetColor, m_dataLocation).toString();
    m_panelBackgroundColor = getSetting(PanelBackgroundColor, m_dataLocation).toString();
    m_buttonHoverBackgroundColor = getSetting(ButtonHoverBackgroundColor, m_dataLocation).toString();
    m_buttonBackgroundColor = getSetting(ButtonBackgroundColor, m_dataLocation).toString();
    m_defaultTextColor = getSetting(ColorTextOnBackground, m_dataLocation).toString();
    m_grayTextColor = getSetting(GrayTextColor, m_dataLocation).toString();
    m_disabledColor = getSetting(DisabledColor, m_dataLocation).toString();
    m_windowBorderWidth = getSetting(WindowBorderWidth, m_dataLocation).toString();
    m_settings->endGroup();

    L_INFO("End to read all settings");
}

QString Settings::paramListString()
{
    QString s;

    s = s + "app_name = " + m_appName + "\n";
    s = s + "updater_version = " + m_updaterVersion + "\n";
    s = s + "java_version = " + m_javaVersion + "\n";
    s = s + "proxy_use = " + QString::number(m_proxyUse) + "\n";
    s = s + "proxy_hostname = " + m_proxyHostname + "\n";
    s = s + "proxy_port = " + QString::number(m_proxyPort) + "\n";
    s = s + "proxy_login = " + m_proxyLogin + "\n";
    s = s + "proxy_password (encrypted) = " + m_proxyPassword + "\n";
    s = s + "lang = " + m_lang + "\n";
    s = s + "shortcut_offline = " + QString::number(m_shortcutOffline) + "\n";
    s = s + "shortcut_online = " + QString::number(m_shortcutOnline) + "\n";
    s = s + "shortcut_name = " + m_shortcutName + "\n";
    s = s + "shortcut_offine_name = " + m_shortcutOfflineName + "\n";
    s = s + "shortcut_offline_args = " + m_shortcutOfflineArgs + "\n";
    s = s + "shortcut_for_all_users = " + QString::number(m_shortcutForAllUsers) + "\n";
    s = s + "data_location = " + m_dataLocation + "\n";
    s = s + "deployment_url = " + m_deploymentUrl + "\n";
    s = s + "run_at_start = " + QString::number(m_runAtStart) + "\n";

    return s;
}

QString Settings::getWindowBorderWidth() const
{
    return m_windowBorderWidth;
}

void Settings::setWindowBorderWidth(const QString &windowBorderWidth)
{
    m_windowBorderWidth = windowBorderWidth;
}

QString Settings::getDisabledColor() const
{
    return m_disabledColor;
}

void Settings::setDisabledColor(const QString &disabledColor)
{
    m_disabledColor = disabledColor;
}

QString Settings::getGrayTextColor() const
{
    return m_grayTextColor;
}

void Settings::setGrayTextColor(const QString &grayTextColor)
{
    m_grayTextColor = grayTextColor;
}

QString Settings::getDefaultTextColor() const
{
    return m_defaultTextColor;
}

void Settings::setDefaultTextColor(const QString &defaultTextColor)
{
    m_defaultTextColor = defaultTextColor;
}

QString Settings::getButtonBackgroundColor() const
{
    return m_buttonBackgroundColor;
}

void Settings::setButtonBackgroundColor(const QString &buttonBackgroundColor)
{
    m_buttonBackgroundColor = buttonBackgroundColor;
}

QString Settings::getButtonHoverBackgroundColor() const
{
    return m_buttonHoverBackgroundColor;
}

void Settings::setButtonHoverBackgroundColor(const QString &buttonHoverBackgroundColor)
{
    m_buttonHoverBackgroundColor = buttonHoverBackgroundColor;
}

QString Settings::getPanelBackgroundColor() const
{
    return m_panelBackgroundColor;
}

void Settings::setPanelBackgroundColor(const QString &panelBackgroundColor)
{
    m_panelBackgroundColor = panelBackgroundColor;
}

QString Settings::getInsetColor() const
{
    return m_insetColor;
}

void Settings::setInsetColor(const QString &insetColor)
{
    m_insetColor = insetColor;
}

QString Settings::getShortcutOfflineArgs() const
{
    return m_shortcutOfflineArgs;
}

void Settings::setShortcutOfflineArgs(const QString &shortcutOfflineArgs)
{
    m_shortcutOfflineArgs = shortcutOfflineArgs;
}

QString Settings::getShortcutOfflineName() const
{
    return m_shortcutOfflineName;
}

void Settings::setShortcutOfflineName(const QString &shortcutOfflineName)
{
    m_shortcutOfflineName = shortcutOfflineName;
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

bool Settings::isShortcutForAllUsers() const
{
    return m_shortcutForAllUsers;
}

void Settings::setShortcutForAllUsers(bool shortcutForAllUsers)
{
    m_shortcutForAllUsers = shortcutForAllUsers;
}

bool Settings::isShortcutOnline() const
{
    return m_shortcutOnline;
}

void Settings::setShortcutOnline(bool shortcutOnline)
{
    m_shortcutOnline = shortcutOnline;
}

bool Settings::isShortcutOffline() const
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

Language Settings::getLang() const
{
    return LanguageManager::getLanguageFromLocale(m_lang);
}

void Settings::setLang(const Language &lang)
{
    m_lang = LanguageManager::getLocaleFromLanguage(lang);
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

bool Settings::isProxyUse() const
{
    return m_proxyUse;
}

void Settings::setProxyUse(bool proxyUse)
{
    m_proxyUse = proxyUse;
}
