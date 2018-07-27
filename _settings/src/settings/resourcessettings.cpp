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
const QString ResourcesSettings::ShortcutOfflineName("shortcut_name_offline");
const QString ResourcesSettings::ShortcutOnline("shortcut_online");
const QString ResourcesSettings::ShortcutOffline("shortcut_offline");
const QString ResourcesSettings::ShortcutOfflineArgs("shortcut_offline_args");

const QString ResourcesSettings::RunAtStart("run_at_start");

const QString ResourcesSettings::DefaultInstallationPath("default_installation_path");
const QString ResourcesSettings::DefaultInstallationFolderName("default_installation_folder_name");

const QString ResourcesSettings::DefaultSimpleInstallDataPath("default_data_path_simple_install");
const QString ResourcesSettings::DefaultCustomInstallDataPath("default_data_path_custom_install");
const QString ResourcesSettings::ChangeDataLocationAllowed("possible_change_data_location");

const QString ResourcesSettings::EncryptedPasswordKey("encrypted_password_key");

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
    m_defaultInstallationFolderName("Application"),
    m_defaultSimpleInstallDataPath("$INSTALL_PATH/Data"),
    m_defaultCustomInstallDataPath("$INSTALL_PATH/Data"),
    m_changeDataLocationAllowed(false),
    m_encryptedPasswordKey("0x0c2cd4a4bcb9f023")
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
    m_defaultInstallationFolderName = m_settings->value(DefaultInstallationFolderName, m_defaultInstallationFolderName).toString();

    m_defaultSimpleInstallDataPath = getTransformedVariablePath(m_settings->value(DefaultSimpleInstallDataPath, m_defaultSimpleInstallDataPath).toString());
    m_defaultCustomInstallDataPath = getTransformedVariablePath(m_settings->value(DefaultCustomInstallDataPath, m_defaultCustomInstallDataPath).toString());
    m_changeDataLocationAllowed = m_settings->value(ChangeDataLocationAllowed, m_changeDataLocationAllowed).toBool();

    m_encryptedPasswordKey = m_settings->value(EncryptedPasswordKey, m_encryptedPasswordKey).toString();

    m_insetColor = m_settings->value(InsetColor, m_insetColor).toString();
    m_panelBackgroundColor = m_settings->value(PanelBackgroundColor, m_panelBackgroundColor).toString();
    m_buttonHoverBackgroundColor = m_settings->value(ButtonHoverBackgroundColor, m_buttonHoverBackgroundColor).toString();
    m_buttonBackgroundColor = m_settings->value(ButtonBackgroundColor, m_buttonBackgroundColor).toString();
    m_defaultTextColor = m_settings->value(DefaultTextColor, m_defaultTextColor).toString();
    m_grayTextColor = m_settings->value(GrayTextColor, m_grayTextColor).toString();
    m_disabledColor = m_settings->value(DisabledColor, m_disabledColor).toString();
    m_windowBorderWidth = m_settings->value(WindowBorderWidth, m_windowBorderWidth).toString();
}

void ResourcesSettings::sendToSettings()
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

    // install path
    settings->setInstallLocation(m_defaultInstallationPath + QDir::separator() + m_defaultInstallationFolderName);

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

void ResourcesSettings::setRunAtStart(bool runAtStart)
{
    m_runAtStart = runAtStart;
}

QString ResourcesSettings::getShortcutOfflineArgs() const
{
    return m_shortcutOfflineArgs;
}

void ResourcesSettings::setShortcutOfflineArgs(const QString &shortcutOfflineArgs)
{
    m_shortcutOfflineArgs = shortcutOfflineArgs;
}

QString ResourcesSettings::getDefaultInstallationFolderName() const
{
    return m_defaultInstallationFolderName;
}

void ResourcesSettings::setDefaultInstallationFolderName(const QString &defaultInstallationFolderName)
{
    m_defaultInstallationFolderName = defaultInstallationFolderName;
}

QString ResourcesSettings::getEncryptedPasswordKey() const
{
    return m_encryptedPasswordKey;
}

void ResourcesSettings::setEncryptedPasswordKey(const QString &encryptedPasswordKey)
{
    m_encryptedPasswordKey = encryptedPasswordKey;
}

QString ResourcesSettings::getDeploymentUrl() const
{
    return m_deploymentUrl;
}

void ResourcesSettings::setDeploymentUrl(const QString &deploymentUrl)
{
    m_deploymentUrl = deploymentUrl;
}

QString ResourcesSettings::getAppName() const
{
    return m_appName;
}

void ResourcesSettings::setAppName(const QString &appName)
{
    m_appName = appName;
}

QString ResourcesSettings::getLang() const
{
    return m_lang;
}

void ResourcesSettings::setLang(const QString &lang)
{
    m_lang = lang;
}

QString ResourcesSettings::getShortcutName() const
{
    return m_shortcutName;
}

void ResourcesSettings::setShortcutName(const QString &shortcutName)
{
    m_shortcutName = shortcutName;
}

QString ResourcesSettings::getShortcutOfflineName() const
{
    return m_shortcutOfflineName;
}

void ResourcesSettings::setShortcutOfflineName(const QString &shortcutOfflineName)
{
    m_shortcutOfflineName = shortcutOfflineName;
}

bool ResourcesSettings::isShortcutOnline() const
{
    return m_shortcutOnline;
}

void ResourcesSettings::setShortcutOnline(bool shortcutOnline)
{
    m_shortcutOnline = shortcutOnline;
}

bool ResourcesSettings::isShortcutOffline() const
{
    return m_shortcutOffline;
}

void ResourcesSettings::setShortcutOffline(bool shortcutOffline)
{
    m_shortcutOffline = shortcutOffline;
}

QString ResourcesSettings::getDefaultInstallationPath() const
{
    return m_defaultInstallationPath;
}

void ResourcesSettings::setDefaultInstallationPath(const QString &defaultInstallationPath)
{
    m_defaultInstallationPath = defaultInstallationPath;
}

bool ResourcesSettings::isChangeDataLocationAllowed() const
{
    return m_changeDataLocationAllowed;
}

void ResourcesSettings::setChangeDataLocationAllowed(bool changeDataLocationAllowed)
{
    m_changeDataLocationAllowed = changeDataLocationAllowed;
}

QString ResourcesSettings::getDefaultCustomInstallDataPath() const
{
    return m_defaultCustomInstallDataPath;
}

void ResourcesSettings::setDefaultCustomInstallDataPath(const QString &defaultCustomInstallDataPath)
{
    m_defaultCustomInstallDataPath = defaultCustomInstallDataPath;
}

QString ResourcesSettings::getDefaultSimpleInstallDataPath() const
{
    return m_defaultSimpleInstallDataPath;
}

void ResourcesSettings::setDefaultSimpleInstallDataPath(const QString &defaultSimpleInstallDataPath)
{
    m_defaultSimpleInstallDataPath = defaultSimpleInstallDataPath;
}

QString ResourcesSettings::getInsetColor() const
{
    return m_insetColor;
}

void ResourcesSettings::setInsetColor(const QString &insetColor)
{
    m_insetColor = insetColor;
}

QString ResourcesSettings::getPanelBackgroundColor() const
{
    return m_panelBackgroundColor;
}

void ResourcesSettings::setPanelBackgroundColor(const QString &panelBackgroundColor)
{
    m_panelBackgroundColor = panelBackgroundColor;
}

QString ResourcesSettings::getButtonHoverBackgroundColor() const
{
    return m_buttonHoverBackgroundColor;
}

void ResourcesSettings::setButtonHoverBackgroundColor(const QString &buttonHoverBackgroundColor)
{
    m_buttonHoverBackgroundColor = buttonHoverBackgroundColor;
}

QString ResourcesSettings::getButtonBackgroundColor() const
{
    return m_buttonBackgroundColor;
}

void ResourcesSettings::setButtonBackgroundColor(const QString &buttonBackgroundColor)
{
    m_buttonBackgroundColor = buttonBackgroundColor;
}

QString ResourcesSettings::getDefaultTextColor() const
{
    return m_defaultTextColor;
}

void ResourcesSettings::setDefaultTextColor(const QString &defaultTextColor)
{
    m_defaultTextColor = defaultTextColor;
}

QString ResourcesSettings::getGrayTextColor() const
{
    return m_grayTextColor;
}

void ResourcesSettings::setGrayTextColor(const QString &grayTextColor)
{
    m_grayTextColor = grayTextColor;
}

QString ResourcesSettings::getDisabledColor() const
{
    return m_disabledColor;
}

void ResourcesSettings::setDisabledColor(const QString &disabledColor)
{
    m_disabledColor = disabledColor;
}

QString ResourcesSettings::getWindowBorderWidth() const
{
    return m_windowBorderWidth;
}

void ResourcesSettings::setWindowBorderWidth(const QString &windowBorderWidth)
{
    m_windowBorderWidth = windowBorderWidth;
}
