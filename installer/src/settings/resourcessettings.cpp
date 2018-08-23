#include "settings/resourcessettings.h"
#include "log/logger.h"
#include "settings/settings.h"
#include "lang/languagemanager.h"

#include <QSettings>
#include <QStandardPaths>
#include <QDir>

const QString ResourcesSettings::DeploymentUrl("deployment_url");
const QString ResourcesSettings::AppName("app_name");
const QString ResourcesSettings::Lang("lang");

const QString ResourcesSettings::ShortcutName("shortcut_name");
const QString ResourcesSettings::ShortcutOfflineName("shortcut_offline_name");
const QString ResourcesSettings::ShortcutOnline("shortcut_online");
const QString ResourcesSettings::ShortcutOffline("shortcut_offline");
const QString ResourcesSettings::ShortcutOfflineArgs("shortcut_offline_args");

const QString ResourcesSettings::RunAtStart("run_at_start");

const QString ResourcesSettings::DefaultInstallationPath("default_installation_path");

const QString ResourcesSettings::DefaultSimpleInstallDataPath("default_simple_install_data_path");
const QString ResourcesSettings::DefaultCustomInstallDataPath("default_custom_install_path");
const QString ResourcesSettings::ChangeDataLocationAllowed("change_data_location_allowed");

const QString ResourcesSettings::InsetColor("inset_color");
const QString ResourcesSettings::PanelBackgroundColor("panel_background_color");
const QString ResourcesSettings::ButtonHoverBackgroundColor("button_hover_background_color");
const QString ResourcesSettings::ButtonBackgroundColor("button_background_color");
const QString ResourcesSettings::DefaultTextColor("default_text_color");
const QString ResourcesSettings::GrayTextColor("gray_text_color");
const QString ResourcesSettings::DisabledColor("disabled_color");
const QString ResourcesSettings::WindowBorderWidth("window_border_width");

ResourcesSettings * ResourcesSettings::sm_instance = 0;
QMutex ResourcesSettings::sm_instanceMutex;
QMutex ResourcesSettings::sm_settingsMutex;

ResourcesSettings::ResourcesSettings() :
    m_settings(0),
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
    L_INFO("Initialise ResourcesSettings singleton instance");
}

ResourcesSettings::~ResourcesSettings()
{
    delete m_settings;
}

ResourcesSettings * ResourcesSettings::getInstance()
{
    if (ResourcesSettings::sm_instance == 0) {
        ResourcesSettings::sm_instanceMutex.lock();
        if (ResourcesSettings::sm_instance == 0) {
            ResourcesSettings::sm_instance = new ResourcesSettings();
        }
        ResourcesSettings::sm_instanceMutex.unlock();
    }
    return ResourcesSettings::sm_instance;
}

void ResourcesSettings::kill()
{
    if (ResourcesSettings::sm_instance) {
        ResourcesSettings::sm_instanceMutex.lock();
        if (ResourcesSettings::sm_instance) {
            delete ResourcesSettings::sm_instance;
            ResourcesSettings::sm_instance = 0;
        }
        ResourcesSettings::sm_instanceMutex.unlock();
    }
}

void ResourcesSettings::initSettings(QString _appPath)
{
    if (m_settings == 0) {
        m_settings = new QSettings(_appPath, QSettings::IniFormat);
        m_settings->setFallbacksEnabled(false);
    }
}

void ResourcesSettings::readSettings()
{
    L_INFO("Starting to read all settings");
    QMutexLocker locker(&sm_settingsMutex);

    m_deploymentUrl = m_settings->value(DeploymentUrl, m_deploymentUrl).toString();
    m_appName = m_settings->value(AppName, m_appName).toString();
    m_lang = m_settings->value(Lang, m_lang).toString();

    m_shortcutName = m_settings->value(ShortcutName, m_shortcutName).toString();
    m_shortcutOfflineName = m_settings->value(ShortcutOfflineName, m_shortcutOfflineName).toString();
    m_shortcutOnline = m_settings->value(ShortcutOnline, m_shortcutOnline).toBool();
    m_shortcutOffline = m_settings->value(ShortcutOffline, m_shortcutOffline).toBool();
    m_shortcutOfflineArgs = m_settings->value(ShortcutOfflineArgs, m_shortcutOfflineArgs).toString();

    m_runAtStart = m_settings->value(RunAtStart, m_runAtStart).toBool();

    m_defaultInstallationPath = getTransformedVariablePath(m_settings->value(DefaultInstallationPath, m_defaultInstallationPath).toString());

    m_defaultSimpleInstallDataPath = getTransformedVariablePath(m_settings->value(DefaultSimpleInstallDataPath, m_defaultSimpleInstallDataPath).toString());
    m_defaultCustomInstallDataPath = getTransformedVariablePath(m_settings->value(DefaultCustomInstallDataPath, m_defaultCustomInstallDataPath).toString());
    m_changeDataLocationAllowed = m_settings->value(ChangeDataLocationAllowed, m_changeDataLocationAllowed).toBool();

    m_insetColor = m_settings->value(InsetColor, m_insetColor).toString();
    m_panelBackgroundColor = m_settings->value(PanelBackgroundColor, m_panelBackgroundColor).toString();
    m_buttonHoverBackgroundColor = m_settings->value(ButtonHoverBackgroundColor, m_buttonHoverBackgroundColor).toString();
    m_buttonBackgroundColor = m_settings->value(ButtonBackgroundColor, m_buttonBackgroundColor).toString();
    m_defaultTextColor = m_settings->value(DefaultTextColor, m_defaultTextColor).toString();
    m_grayTextColor = m_settings->value(GrayTextColor, m_grayTextColor).toString();
    m_disabledColor = m_settings->value(DisabledColor, m_disabledColor).toString();
    m_windowBorderWidth = m_settings->value(WindowBorderWidth, m_windowBorderWidth).toString();
}

void ResourcesSettings::writeAppSettings()
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

QString ResourcesSettings::getTransformedVariablePath(QString _path)
{
    _path.replace(QString("$HOME"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    _path.replace(QString("$INSTALL_PATH"), m_defaultInstallationPath);
    // TODO attention sous macos !
    _path.replace(QString("$APPDATA_JAVA_TMP"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/AppData/LocalLow/Sun/Java/Deployment/tmp");
    return _path;
}

bool ResourcesSettings::isRunAtStart() const
{
    return m_runAtStart;
}

QString ResourcesSettings::getShortcutOfflineArgs() const
{
    return m_shortcutOfflineArgs;
}

QString ResourcesSettings::getDeploymentUrl() const
{
    return m_deploymentUrl;
}

QString ResourcesSettings::getAppName() const
{
    return m_appName;
}

QString ResourcesSettings::getLang() const
{
    return m_lang;
}

QString ResourcesSettings::getShortcutName() const
{
    return m_shortcutName;
}

QString ResourcesSettings::getShortcutOfflineName() const
{
    return m_shortcutOfflineName;
}

bool ResourcesSettings::isShortcutOnline() const
{
    return m_shortcutOnline;
}

bool ResourcesSettings::isShortcutOffline() const
{
    return m_shortcutOffline;
}

QString ResourcesSettings::getDefaultInstallationPath() const
{
    return m_defaultInstallationPath;
}

bool ResourcesSettings::isChangeDataLocationAllowed() const
{
    return m_changeDataLocationAllowed;
}

QString ResourcesSettings::getDefaultCustomInstallDataPath() const
{
    return m_defaultCustomInstallDataPath;
}

QString ResourcesSettings::getDefaultSimpleInstallDataPath() const
{
    return m_defaultSimpleInstallDataPath;
}

QString ResourcesSettings::getInsetColor() const
{
    return m_insetColor;
}

QString ResourcesSettings::getPanelBackgroundColor() const
{
    return m_panelBackgroundColor;
}

QString ResourcesSettings::getButtonHoverBackgroundColor() const
{
    return m_buttonHoverBackgroundColor;
}

QString ResourcesSettings::getButtonBackgroundColor() const
{
    return m_buttonBackgroundColor;
}

QString ResourcesSettings::getDefaultTextColor() const
{
    return m_defaultTextColor;
}

QString ResourcesSettings::getGrayTextColor() const
{
    return m_grayTextColor;
}

QString ResourcesSettings::getDisabledColor() const
{
    return m_disabledColor;
}

QString ResourcesSettings::getWindowBorderWidth() const
{
    return m_windowBorderWidth;
}
