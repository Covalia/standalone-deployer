#include "settings/settings.h"
#include "log/logger.h"
#include "utils/crypt/cryptmanager.h"
#include "lang/languagemanager.h"

#include <QMutexLocker>
#include <QSettings>

const QString Settings::GroupInfo("INFO");
const QString Settings::GroupProxy("PROXY");
const QString Settings::GroupLang("LANG");
const QString Settings::GroupShortcut("SHORTCUT");
const QString Settings::GroupData("DATA");
const QString Settings::GroupServer("SERVER");
const QString Settings::GroupStart("START");
const QString Settings::GroupTheme("THEME");

const QString Settings::AppName("app_name");
const QString Settings::UpdaterVersion("updater_version");
const QString Settings::JavaVersion("java_version");

const QString Settings::ProxyUse("proxy_use");
const QString Settings::ProxyAuto("proxy_auto");
const QString Settings::ProxyManual("proxy_manual");
const QString Settings::ProxyHostname("proxy_hostname");
const QString Settings::ProxyPort("proxy_port");
const QString Settings::ProxyAuthentication("proxy_authentication");
const QString Settings::ProxyLogin("proxy_login");
const QString Settings::ProxyPassword("proxy_password");

const QString Settings::Lang("lang");

const QString Settings::ShortcutOnline("shortcut_online");
const QString Settings::ShortcutOffline("shortcut_offline");
const QString Settings::ShortcutName("shortcut_name");
const QString Settings::ShortcutOfflineName("shortcut_offline_name");
const QString Settings::ShortcutOfflineArgs("shortcut_offline_args");
const QString Settings::ShortcutForAllUsers("shortcut_for_all_users");

const QString Settings::InstallLocation("install_location");
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
    m_installLocation(""),
    m_updaterVersion(""),
    m_javaVersion(""),
    m_proxyUse(false),
    m_proxyAuto(false),
    m_proxyManual(false),
    m_proxyHostname(""),
    m_proxyPort(-1),
    m_proxyAuthentification(false),
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

    m_settings->beginGroup(GroupInfo);
    putSetting(AppName, m_appName);
    putSetting(UpdaterVersion, m_updaterVersion);
    putSetting(JavaVersion, m_javaVersion);
    m_settings->endGroup();

    m_settings->beginGroup(GroupProxy);
    putSetting(ProxyUse, m_proxyUse);
    putSetting(ProxyAuto, m_proxyAuto);
    putSetting(ProxyManual, m_proxyManual);
    putSetting(ProxyHostname, m_proxyHostname);
    putSetting(ProxyPort, m_proxyPort);
    putSetting(ProxyAuthentication, m_proxyAuthentification);
    putSetting(ProxyLogin, m_proxyLogin);
    putSetting(ProxyPassword, m_proxyPassword);
    m_settings->endGroup();

    m_settings->beginGroup(GroupLang);
    putSetting(Lang, m_lang);
    m_settings->endGroup();

    m_settings->beginGroup(GroupShortcut);
    putSetting(ShortcutOffline, m_shortcutOffline);
    putSetting(ShortcutOnline, m_shortcutOnline);
    putSetting(ShortcutName, m_shortcutName);
    putSetting(ShortcutOfflineName, m_shortcutOfflineName);
    putSetting(ShortcutOfflineArgs, m_shortcutOfflineArgs);
    putSetting(ShortcutForAllUsers, m_shortcutForAllUsers);
    m_settings->endGroup();

    m_settings->beginGroup(GroupData);
    putSetting(DataLocation, m_dataLocation);
    putSetting(InstallLocation, m_installLocation);
    m_settings->endGroup();

    m_settings->beginGroup(GroupServer);
    putSetting(DeploymentUrl, m_deploymentUrl);
    m_settings->endGroup();

    m_settings->beginGroup(GroupStart);
    putSetting(RunAtStart, m_runAtStart);
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

    m_settings->beginGroup(GroupInfo);
    m_appName = getSetting(AppName, m_appName).toString();
    m_updaterVersion = getSetting(UpdaterVersion, m_updaterVersion).toString();
    m_javaVersion = getSetting(JavaVersion, m_javaVersion).toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupProxy);
    m_proxyUse = getSetting(ProxyUse, m_proxyUse).toBool();
    m_proxyAuto = getSetting(ProxyAuto, m_proxyAuto).toBool();
    m_proxyManual = getSetting(ProxyManual, m_proxyManual).toBool();
    m_proxyHostname = getSetting(ProxyHostname, "").toString();
    m_proxyPort = getSetting(ProxyPort, 0).toInt();
    m_proxyAuthentification = getSetting(ProxyAuthentication, m_proxyAuthentification).toBool();
    m_proxyLogin = getSetting(ProxyLogin, "").toString();
    m_proxyPassword = getSetting(ProxyPassword, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupLang);
    m_lang = getSetting(Lang, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupShortcut);
    m_shortcutOffline = getSetting(ShortcutOffline, m_shortcutOffline).toBool();
    m_shortcutOnline = getSetting(ShortcutOnline, m_shortcutOnline).toBool();
    m_shortcutName = getSetting(ShortcutName, m_shortcutName).toString();
    m_shortcutOfflineName = getSetting(ShortcutOfflineName, m_shortcutOfflineName).toString();
    m_shortcutOfflineArgs = getSetting(ShortcutOfflineArgs, m_shortcutOfflineArgs).toString();
    m_shortcutForAllUsers = getSetting(ShortcutForAllUsers, m_shortcutForAllUsers).toBool();
    m_settings->endGroup();

    m_settings->beginGroup(GroupData);
    m_dataLocation = getSetting(DataLocation, m_dataLocation).toString();
    m_installLocation = getSetting(InstallLocation, m_installLocation).toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupServer);
    m_deploymentUrl = getSetting(DeploymentUrl, "").toString();
    m_settings->endGroup();

    m_settings->beginGroup(GroupStart);
    m_runAtStart = getSetting(RunAtStart, m_runAtStart).toBool();
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
    s = s + "proxy_auto = " + QString::number(m_proxyAuto) + "\n";
    s = s + "proxy_manual = " + QString::number(m_proxyManual)  + "\n";
    s = s + "proxy_hostname = " + m_proxyHostname + "\n";
    s = s + "proxy_port = " + QString::number(m_proxyPort) + "\n";
    s = s + "proxy_authentification = " + QString::number(m_proxyAuthentification) + "\n";
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
    s = s + "install_location = " + m_installLocation + "\n";
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
    CryptManager * crypt = new CryptManager();

    return crypt->decryptToString(m_proxyPassword);
}

void Settings::setProxyPassword(const QString &proxyPassword)
{
    CryptManager * crypt = new CryptManager();

    m_proxyPassword = crypt->encryptToString(proxyPassword);
}

QString Settings::getProxyLogin() const
{
    return m_proxyLogin;
}

void Settings::setProxyLogin(const QString &proxyLogin)
{
    m_proxyLogin = proxyLogin;
}

bool Settings::isProxyAuthentification() const
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

QString Settings::getProxyHostname() const
{
    return m_proxyHostname;
}

void Settings::setProxyHostname(const QString &proxyHostname)
{
    m_proxyHostname = proxyHostname;
}

bool Settings::isProxyManual() const
{
    return m_proxyManual;
}

void Settings::setProxyManual(bool proxyManual)
{
    m_proxyManual = proxyManual;
}

bool Settings::isProxyAuto() const
{
    return m_proxyAuto;
}

void Settings::setProxyAuto(bool proxyAuto)
{
    m_proxyAuto = proxyAuto;
}

bool Settings::isProxyUse() const
{
    return m_proxyUse;
}

void Settings::setProxyUse(bool proxyUse)
{
    m_proxyUse = proxyUse;
}
