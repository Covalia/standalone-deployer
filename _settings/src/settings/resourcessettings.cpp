#include "settings/resourcessettings.h"
#include "log/logger.h"
#include "settings/settings.h"
#include "lang/languagemanager.h"

#include <QSettings>
#include <QStandardPaths>
#include <QDir>

ResourcesSettings * ResourcesSettings::sm_instance = 0;
QMutex ResourcesSettings::sm_instanceMutex;
QMutex ResourcesSettings::sm_settingsMutex;

ResourcesSettings::ResourcesSettings() :
    m_settings(0),
    m_ip_server(""),
    m_app_name("Application"),
    m_language("en_US"),
    m_shortcut_name("Application"),
    m_shortcut_name_offline("Application Offline"),
    m_shortcut_online(true),
    m_shortcut_offline(false),
    m_shortcut_offline_args("--offline=true"),
    m_run_at_start(true),
    m_default_installation_path("$HOME"),
    m_default_installation_folder_name("Application"),
    m_default_data_path_simple_install("$INSTALL_PATH/Data"),
    m_default_data_path_custom_install("$INSTALL_PATH/Data"),
    m_possible_change_data_location(false),
    m_hash_key(""),
    m_encrypted_password_key("0x0c2cd4a4bcb9f023"),
    m_color_panel_background_border("#364058"),
    m_color_panel_background("#2d364c"),
    m_color_button_background_over("#2a7d7d"),
    m_color_button_background("#339999"),
    m_color_text_on_background("#eff0f2"),
    m_color_text_gray("#9ea0a5"),
    m_color_disabled("#656976"),
    m_border_window("0")
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

    m_ip_server = m_settings->value(P_IP_SERVER, m_ip_server).toString();
    m_app_name = m_settings->value(P_APPLICATION_NAME, m_app_name).toString();
    m_language = m_settings->value(P_LANGUAGE, m_language).toString();

    m_shortcut_name = m_settings->value(P_SHORTCUT_NAME, m_shortcut_name).toString();
    m_shortcut_name_offline = m_settings->value(P_SHORTCUT_NAME_OFFLINE, m_shortcut_name_offline).toString();
    m_shortcut_online = m_settings->value(P_SHORTCUT_ONLINE, m_shortcut_online).toBool();
    m_shortcut_offline = m_settings->value(P_SHORTCUT_OFFLINE, m_shortcut_offline).toBool();
    m_shortcut_offline_args = m_settings->value(P_SHORTCUT_OFFLINE_ARGS, m_shortcut_offline_args).toString();

    m_run_at_start = m_settings->value(P_RUN_AT_START, m_run_at_start).toBool();

    m_default_installation_path = getTransformedVariablePath(m_settings->value(P_DEFAULT_INSTALLATION_PATH, m_default_installation_path).toString());
    m_default_installation_folder_name = m_settings->value(P_DEFAULT_INSTALLATION_FOLDER_NAME, m_default_installation_folder_name).toString();

    m_default_data_path_simple_install = getTransformedVariablePath(m_settings->value(P_DEFAULT_DATA_PATH_SIMPLE_INSTALL, m_default_data_path_simple_install).toString());
    m_default_data_path_custom_install = getTransformedVariablePath(m_settings->value(P_DEFAULT_DATA_PATH_CUSTOM_INSTALL, m_default_data_path_custom_install).toString());
    m_possible_change_data_location = m_settings->value(P_POSSIBLE_CHANGE_DATA_LOCATION, m_possible_change_data_location).toBool();

    m_hash_key = m_settings->value(P_HASH_KEY, m_hash_key).toString();
    m_encrypted_password_key = m_settings->value(P_ENCRYPTED_PASSWORD_KEY, m_encrypted_password_key).toString();

    m_color_panel_background_border = m_settings->value(P_COLOR_PANEL_BACKGROUND_BORDER, m_color_panel_background_border).toString();
    m_color_panel_background = m_settings->value(P_COLOR_PANEL_BACKGROUND, m_color_panel_background).toString();
    m_color_button_background_over = m_settings->value(P_COLOR_BUTTON_BACKGROUND_OVER, m_color_button_background_over).toString();
    m_color_button_background = m_settings->value(P_COLOR_BUTTON_BACKGROUND, m_color_button_background).toString();
    m_color_text_on_background = m_settings->value(P_COLOR_TEXT_ON_BACKGROUND, m_color_text_on_background).toString();
    m_color_text_gray = m_settings->value(P_COLOR_TEXT_GRAY, m_color_text_gray).toString();
    m_color_disabled = m_settings->value(P_COLOR_DISABLED, m_color_disabled).toString();
    m_border_window = m_settings->value(P_BORDER_WINDOW, m_border_window).toString();
}

void ResourcesSettings::sendToSettings()
{
    Settings * settings = Settings::getInstance();

    settings->setApplicationName(m_app_name);
    settings->setServerURL(m_ip_server);
    settings->setLanguage(LanguageManager::getLanguageFromLocale(m_language));
    settings->setShortcutName(m_shortcut_name);
    settings->setShortcutOnline(m_shortcut_online);
    settings->setShortcutOffline(m_shortcut_offline);
    settings->setShortcutOfflineName(m_shortcut_name_offline);
    settings->setShortcutOfflineArgs(m_shortcut_offline_args);

    settings->setRunAtStart(m_run_at_start);

    // install path
    settings->setInstallLocation(m_default_installation_path + QDir::separator() + m_default_installation_folder_name);

    // data path
    // use simple by default
    settings->setDataLocation(m_default_data_path_simple_install);

    settings->setColorPanelBackgroundBorder(m_color_panel_background_border);
    settings->setColorPanelBackground(m_color_panel_background);
    settings->setColorButtonBackgroundOver(m_color_button_background_over);
    settings->setColorButtonBackground(m_color_button_background);
    settings->setColorTextOnBackground(m_color_text_on_background);
    settings->setColorTextGray(m_color_text_gray);
    settings->setColorDisabled(m_color_disabled);
    settings->setBorderWindow(m_border_window);


}

QString ResourcesSettings::getTransformedVariablePath(QString _path)
{
    _path.replace(QString("$HOME"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    _path.replace(QString("$INSTALL_PATH"), m_default_installation_path);
    _path.replace(QString("$APPDATA_JAVA_TMP"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/AppData/LocalLow/Sun/Java/Deployment/tmp");
    return _path;
}

bool ResourcesSettings::isRun_at_start() const
{
    return m_run_at_start;
}

void ResourcesSettings::setRun_at_start(bool run_at_start)
{
    m_run_at_start = run_at_start;
}

QString ResourcesSettings::getShortcut_offline_args() const
{
    return m_shortcut_offline_args;
}

void ResourcesSettings::setShortcut_offline_args(const QString &shortcut_offline_args)
{
    m_shortcut_offline_args = shortcut_offline_args;
}

QString ResourcesSettings::getDefault_installation_folder_name() const
{
    return m_default_installation_folder_name;
}

void ResourcesSettings::setDefault_installation_folder_name(const QString &default_installation_folder_name)
{
    m_default_installation_folder_name = default_installation_folder_name;
}

QString ResourcesSettings::getEncrypted_password_key() const
{
    return m_encrypted_password_key;
}

void ResourcesSettings::setEncrypted_password_key(const QString &encrypted_password_key)
{
    m_encrypted_password_key = encrypted_password_key;
}

QString ResourcesSettings::getHash_key() const
{
    return m_hash_key;
}

void ResourcesSettings::setHash_key(const QString &hash_key)
{
    m_hash_key = hash_key;
}

QString ResourcesSettings::getIp_server() const
{
    return m_ip_server;
}

void ResourcesSettings::setIp_server(const QString &ip_server)
{
    m_ip_server = ip_server;
}

QString ResourcesSettings::getApp_name() const
{
    return m_app_name;
}

void ResourcesSettings::setApp_name(const QString &app_name)
{
    m_app_name = app_name;
}

QString ResourcesSettings::getLanguage() const
{
    return m_language;
}

void ResourcesSettings::setLanguage(const QString &language)
{
    m_language = language;
}

QString ResourcesSettings::getShortcut_name() const
{
    return m_shortcut_name;
}

void ResourcesSettings::setShortcut_name(const QString &shortcut_name)
{
    m_shortcut_name = shortcut_name;
}

QString ResourcesSettings::getShortcut_name_offline() const
{
    return m_shortcut_name_offline;
}

void ResourcesSettings::setShortcut_name_offline(const QString &shortcut_name_offline)
{
    m_shortcut_name_offline = shortcut_name_offline;
}

bool ResourcesSettings::isShortcut_online() const
{
    return m_shortcut_online;
}

void ResourcesSettings::setShortcut_online(bool shortcut_online)
{
    m_shortcut_online = shortcut_online;
}

bool ResourcesSettings::isShortcut_offline() const
{
    return m_shortcut_offline;
}

void ResourcesSettings::setShortcut_offline(bool shortcut_offline)
{
    m_shortcut_offline = shortcut_offline;
}

QString ResourcesSettings::getDefault_installation_path() const
{
    return m_default_installation_path;
}

void ResourcesSettings::setDefault_installation_path(const QString &default_installation_path)
{
    m_default_installation_path = default_installation_path;
}

bool ResourcesSettings::isPossible_change_data_location() const
{
    return m_possible_change_data_location;
}

void ResourcesSettings::setPossible_change_data_location(bool possible_change_data_location)
{
    m_possible_change_data_location = possible_change_data_location;
}

QString ResourcesSettings::getDefault_data_path_custom_install() const
{
    return m_default_data_path_custom_install;
}

void ResourcesSettings::setDefault_data_path_custom_install(const QString &default_data_path_custom_install)
{
    m_default_data_path_custom_install = default_data_path_custom_install;
}

QString ResourcesSettings::getDefault_data_path_simple_install() const
{
    return m_default_data_path_simple_install;
}

void ResourcesSettings::setDefault_data_path_simple_install(const QString &default_data_path_simple_install)
{
    m_default_data_path_simple_install = default_data_path_simple_install;
}

QString ResourcesSettings::getColor_panel_background_border() const
{
    return m_color_panel_background_border;
}

void ResourcesSettings::setColor_panel_background_border(const QString &color_panel_background_border)
{
    m_color_panel_background_border = color_panel_background_border;
}

QString ResourcesSettings::getColor_panel_background() const
{
    return m_color_panel_background;
}

void ResourcesSettings::setColor_panel_background(const QString &color_panel_background)
{
    m_color_panel_background = color_panel_background;
}

QString ResourcesSettings::getColor_button_background_over() const
{
    return m_color_button_background_over;
}

void ResourcesSettings::setColor_button_background_over(const QString &color_button_background_over)
{
    m_color_button_background_over = color_button_background_over;
}

QString ResourcesSettings::getColor_button_background() const
{
    return m_color_button_background;
}

void ResourcesSettings::setColor_button_background(const QString &color_button_background)
{
    m_color_button_background = color_button_background;
}

QString ResourcesSettings::getColor_text_on_background() const
{
    return m_color_text_on_background;
}

void ResourcesSettings::setColor_text_on_background(const QString &color_text_on_background)
{
    m_color_text_on_background = color_text_on_background;
}

QString ResourcesSettings::getColor_text_gray() const
{
    return m_color_text_gray;
}

void ResourcesSettings::setColor_text_gray(const QString &color_text_gray)
{
    m_color_text_gray = color_text_gray;
}

QString ResourcesSettings::getColor_disabled() const
{
    return m_color_disabled;
}

void ResourcesSettings::setColor_disabled(const QString &color_disabled)
{
    m_color_disabled = color_disabled;
}

QString ResourcesSettings::getBorder_window() const
{
    return m_border_window;
}

void ResourcesSettings::setBorder_window(const QString &border_window)
{
    m_border_window = border_window;
}
