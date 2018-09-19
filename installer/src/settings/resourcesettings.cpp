#include "settings/resourcesettings.h"
#include "log/logger.h"
#include "settings/settings.h"
#include "lang/languagemanager.h"

#include <QSettings>
#include <QStandardPaths>
#include <QDir>

const QString ResourceSettings::DeploymentUrl("deployment_url");
const QString ResourceSettings::AppName("app_name");
const QString ResourceSettings::Lang("lang");

const QString ResourceSettings::ShortcutName("shortcut_name");
const QString ResourceSettings::ShortcutOfflineName("shortcut_offline_name");
const QString ResourceSettings::ShortcutOnline("shortcut_online");
const QString ResourceSettings::ShortcutOffline("shortcut_offline");
const QString ResourceSettings::ShortcutOfflineArgs("shortcut_offline_args");

const QString ResourceSettings::RunAtStart("run_at_start");

const QString ResourceSettings::DefaultInstallationPath("default_installation_path");

const QString ResourceSettings::DefaultSimpleInstallDataPath("default_simple_install_data_path");
const QString ResourceSettings::DefaultCustomInstallDataPath("default_custom_install_path");
const QString ResourceSettings::ChangeDataLocationAllowed("change_data_location_allowed");

const QString ResourceSettings::InsetColor("inset_color");
const QString ResourceSettings::PanelBackgroundColor("panel_background_color");
const QString ResourceSettings::ButtonHoverBackgroundColor("button_hover_background_color");
const QString ResourceSettings::ButtonBackgroundColor("button_background_color");
const QString ResourceSettings::DefaultTextColor("default_text_color");
const QString ResourceSettings::GrayTextColor("gray_text_color");
const QString ResourceSettings::DisabledColor("disabled_color");
const QString ResourceSettings::WindowBorderWidth("window_border_width");


ResourceSettings::ResourceSettings(const QString &_appPath) :
    m_settings(_appPath, QSettings::IniFormat),
    m_deploymentUrl(""),
    m_appName("Application"),
    m_lang("en_US"),
    m_shortcutOnline(true),
    m_shortcutOffline(false),
    m_shortcutName("Application"),
    m_shortcutOfflineName("Application Offline"),
    m_shortcutOfflineArgs("--offline=true"),
    m_runAtStart(true),
    m_insetColor("#364058"),
    m_panelBackgroundColor("#2d364c"),
    m_buttonHoverBackgroundColor("#2a7d7d"),
    m_buttonBackgroundColor("#339999"),
    m_defaultTextColor("#eff0f2"),
    m_grayTextColor("#9ea0a5"),
    m_disabledColor("#656976"),
    m_windowBorderWidth("0"),
    m_defaultInstallationPath("$HOME"),
    m_defaultSimpleInstallDataPath("$INSTALL_PATH/data"),
    m_defaultCustomInstallDataPath("$INSTALL_PATH/data"),
    m_changeDataLocationAllowed(false)
{
    L_INFO("Initialise ResourceSettings singleton instance");

    m_settings.setFallbacksEnabled(false);
}

ResourceSettings::~ResourceSettings()
{
}

void ResourceSettings::readSettings()
{
    L_INFO("Starting to read all settings");

    m_deploymentUrl = m_settings.value(DeploymentUrl, m_deploymentUrl).toString();
    m_appName = m_settings.value(AppName, m_appName).toString();
    m_lang = m_settings.value(Lang, m_lang).toString();

    m_shortcutName = m_settings.value(ShortcutName, m_shortcutName).toString();
    m_shortcutOfflineName = m_settings.value(ShortcutOfflineName, m_shortcutOfflineName).toString();
    m_shortcutOnline = m_settings.value(ShortcutOnline, m_shortcutOnline).toBool();
    m_shortcutOffline = m_settings.value(ShortcutOffline, m_shortcutOffline).toBool();
    m_shortcutOfflineArgs = m_settings.value(ShortcutOfflineArgs, m_shortcutOfflineArgs).toString();

    m_runAtStart = m_settings.value(RunAtStart, m_runAtStart).toBool();

    m_defaultInstallationPath = getTransformedVariablePath(m_settings.value(DefaultInstallationPath, m_defaultInstallationPath).toString());

    m_defaultSimpleInstallDataPath = getTransformedVariablePath(m_settings.value(DefaultSimpleInstallDataPath, m_defaultSimpleInstallDataPath).toString());
    m_defaultCustomInstallDataPath = getTransformedVariablePath(m_settings.value(DefaultCustomInstallDataPath, m_defaultCustomInstallDataPath).toString());
    m_changeDataLocationAllowed = m_settings.value(ChangeDataLocationAllowed, m_changeDataLocationAllowed).toBool();

    m_insetColor = m_settings.value(InsetColor, m_insetColor).toString();
    m_panelBackgroundColor = m_settings.value(PanelBackgroundColor, m_panelBackgroundColor).toString();
    m_buttonHoverBackgroundColor = m_settings.value(ButtonHoverBackgroundColor, m_buttonHoverBackgroundColor).toString();
    m_buttonBackgroundColor = m_settings.value(ButtonBackgroundColor, m_buttonBackgroundColor).toString();
    m_defaultTextColor = m_settings.value(DefaultTextColor, m_defaultTextColor).toString();
    m_grayTextColor = m_settings.value(GrayTextColor, m_grayTextColor).toString();
    m_disabledColor = m_settings.value(DisabledColor, m_disabledColor).toString();
    m_windowBorderWidth = m_settings.value(WindowBorderWidth, m_windowBorderWidth).toString();
}

void ResourceSettings::writeAppSettings()
{
    Settings * settings = Settings::getInstance();

    settings->setAppName(m_appName);
    settings->setDeploymentUrl(m_deploymentUrl);
    settings->setLang(LanguageManager::getLanguageFromLocale(m_lang));
    settings->setShortcutName(m_shortcutName);
    settings->setShortcutOnline(m_shortcutOnline);
    settings->setShortcutOffline(m_shortcutOffline);
    settings->setShortcutOfflineName(m_shortcutOfflineName);
    settings->setShortcutOfflineArgs(m_shortcutOfflineArgs);

    settings->setRunAtStart(m_runAtStart);

    // data path
    // use simple by default
    settings->setDataLocation(m_defaultSimpleInstallDataPath);

    settings->setInsetColor(m_insetColor);
    settings->setPanelBackgroundColor(m_panelBackgroundColor);
    settings->setButtonHoverBackgroundColor(m_buttonHoverBackgroundColor);
    settings->setButtonBackgroundColor(m_buttonBackgroundColor);
    settings->setDefaultTextColor(m_defaultTextColor);
    settings->setGrayTextColor(m_grayTextColor);
    settings->setDisabledColor(m_disabledColor);
    settings->setWindowBorderWidth(m_windowBorderWidth);
}

QString ResourceSettings::getTransformedVariablePath(QString _path)
{
    _path.replace(QString("$HOME"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    _path.replace(QString("$INSTALL_PATH"), m_defaultInstallationPath);
    // TODO attention sous macos !
    _path.replace(QString("$APPDATA_JAVA_TMP"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/AppData/LocalLow/Sun/Java/Deployment/tmp");
    return _path;
}

bool ResourceSettings::isRunAtStart() const
{
    return m_runAtStart;
}

QString ResourceSettings::getShortcutOfflineArgs() const
{
    return m_shortcutOfflineArgs;
}

QString ResourceSettings::getDeploymentUrl() const
{
    return m_deploymentUrl;
}

QString ResourceSettings::getAppName() const
{
    return m_appName;
}

QString ResourceSettings::getLang() const
{
    return m_lang;
}

QString ResourceSettings::getShortcutName() const
{
    return m_shortcutName;
}

QString ResourceSettings::getShortcutOfflineName() const
{
    return m_shortcutOfflineName;
}

bool ResourceSettings::isShortcutOnline() const
{
    return m_shortcutOnline;
}

bool ResourceSettings::isShortcutOffline() const
{
    return m_shortcutOffline;
}

QString ResourceSettings::getDefaultInstallationPath() const
{
    return m_defaultInstallationPath;
}

bool ResourceSettings::isChangeDataLocationAllowed() const
{
    return m_changeDataLocationAllowed;
}

QString ResourceSettings::getDefaultCustomInstallDataPath() const
{
    return m_defaultCustomInstallDataPath;
}

QString ResourceSettings::getDefaultSimpleInstallDataPath() const
{
    return m_defaultSimpleInstallDataPath;
}

QString ResourceSettings::getInsetColor() const
{
    return m_insetColor;
}

QString ResourceSettings::getPanelBackgroundColor() const
{
    return m_panelBackgroundColor;
}

QString ResourceSettings::getButtonHoverBackgroundColor() const
{
    return m_buttonHoverBackgroundColor;
}

QString ResourceSettings::getButtonBackgroundColor() const
{
    return m_buttonBackgroundColor;
}

QString ResourceSettings::getDefaultTextColor() const
{
    return m_defaultTextColor;
}

QString ResourceSettings::getGrayTextColor() const
{
    return m_grayTextColor;
}

QString ResourceSettings::getDisabledColor() const
{
    return m_disabledColor;
}

QString ResourceSettings::getWindowBorderWidth() const
{
    return m_windowBorderWidth;
}
