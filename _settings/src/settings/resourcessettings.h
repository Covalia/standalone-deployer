#ifndef SETTINGS__RESOURCESSETTINGS_H
#define SETTINGS__RESOURCESSETTINGS_H

#include <QString>
#include <QSettings>
#include <QMutex>

static QString P_DEPLOYMENT_URL = "deployment_url";
static QString P_APPLICATION_NAME = "application_name";
static QString P_LANGUAGE = "language";

static QString P_SHORTCUT_NAME = "shortcut_name";
static QString P_SHORTCUT_NAME_OFFLINE = "shortcut_name_offline";
static QString P_SHORTCUT_ONLINE = "shortcut_online";
static QString P_SHORTCUT_OFFLINE = "shortcut_offline";
static QString P_SHORTCUT_OFFLINE_ARGS = "shortcut_offline_args";

static QString P_RUN_AT_START = "run_at_start";

static QString P_DEFAULT_INSTALLATION_PATH = "default_installation_path";
static QString P_DEFAULT_INSTALLATION_FOLDER_NAME = "default_installation_folder_name";

static QString P_DEFAULT_DATA_PATH_SIMPLE_INSTALL = "default_data_path_simple_install";
static QString P_DEFAULT_DATA_PATH_CUSTOM_INSTALL = "default_data_path_custom_install";
static QString P_POSSIBLE_CHANGE_DATA_LOCATION = "possible_change_data_location";

static QString P_ENCRYPTED_PASSWORD_KEY = "encrypted_password_key";

static QString P_COLOR_PANEL_BACKGROUND_BORDER = "color_panel_background_border";
static QString P_COLOR_PANEL_BACKGROUND = "color_panel_background";
static QString P_COLOR_BUTTON_BACKGROUND_OVER = "color_button_background_over";
static QString P_COLOR_BUTTON_BACKGROUND = "color_button_background";
static QString P_COLOR_TEXT_ON_BACKGROUND = "color_text_on_background";
static QString P_COLOR_TEXT_GRAY = "color_text_gray";
static QString P_COLOR_DISABLED = "color_disabled";
static QString P_BORDER_WINDOW = "border_window";

class ResourcesSettings
{
    public:
        static ResourcesSettings * getInstance();
        static void kill();

        void initSettings(QString _appPath);

        void readSettings();

        void sendToSettings();

        QString getBorder_window() const;
        void setBorder_window(const QString &border_window);

        QString getColor_disabled() const;
        void setColor_disabled(const QString &color_disabled);

        QString getColor_text_gray() const;
        void setColor_text_gray(const QString &color_text_gray);

        QString getColor_text_on_background() const;
        void setColor_text_on_background(const QString &color_text_on_background);

        QString getColor_button_background() const;
        void setColor_button_background(const QString &color_button_background);

        QString getColor_button_background_over() const;
        void setColor_button_background_over(const QString &color_button_background_over);

        QString getColor_panel_background() const;
        void setColor_panel_background(const QString &color_panel_background);

        QString getColor_panel_background_border() const;
        void setColor_panel_background_border(const QString &color_panel_background_border);

        bool isPossible_change_data_location() const;
        void setPossible_change_data_location(bool possible_change_data_location);

        QString getDefault_installation_path() const;
        void setDefault_installation_path(const QString &default_installation_path);

        bool isShortcut_offline() const;
        void setShortcut_offline(bool shortcut_offline);

        bool isShortcut_online() const;
        void setShortcut_online(bool shortcut_online);

        QString getShortcut_name_offline() const;
        void setShortcut_name_offline(const QString &shortcut_name_offline);

        QString getShortcut_name() const;
        void setShortcut_name(const QString &shortcut_name);

        QString getLanguage() const;
        void setLanguage(const QString &language);

        QString getApp_name() const;
        void setApp_name(const QString &app_name);

        QString getDeployment_Url() const;
        void setDeployment_Url(const QString &deployment_url);

        QString getDefault_data_path_simple_install() const;
        void setDefault_data_path_simple_install(const QString &default_data_path_simple_install);

        QString getDefault_data_path_custom_install() const;
        void setDefault_data_path_custom_install(const QString &default_data_path_custom_install);

        QString getEncrypted_password_key() const;
        void setEncrypted_password_key(const QString &encrypted_password_key);

        QString getDefault_installation_folder_name() const;
        void setDefault_installation_folder_name(const QString &default_installation_folder_name);

        QString getShortcut_offline_args() const;
        void setShortcut_offline_args(const QString &shortcut_offline_args);

        bool isRun_at_start() const;
        void setRun_at_start(bool run_at_start);

private:
        /**
         * @brief Use for singleton
         */
        void operator=(const ResourcesSettings&)
        {
        }
        ResourcesSettings (const ResourcesSettings&)
        {
        }

        static ResourcesSettings * sm_instance;
        static QMutex sm_instanceMutex;
        static QMutex sm_settingsMutex;

        /**
         * @brief Constructor in singleton
         */
        ResourcesSettings();

        /**
         * @brief Destructor in singleton
         */
        virtual ~ResourcesSettings();

        QString getTransformedVariablePath(QString _path);

        /**
         * @brief QSetting instance for find and add setting
         */
        QSettings * m_settings;

        QString m_deployment_url;
        QString m_app_name;
        QString m_language;
        QString m_shortcut_name;
        QString m_shortcut_name_offline;
        bool m_shortcut_online;
        bool m_shortcut_offline;
        QString m_shortcut_offline_args;
        bool m_run_at_start;
        QString m_default_installation_path;
        QString m_default_installation_folder_name;
        QString m_default_data_path_simple_install;
        QString m_default_data_path_custom_install;
        bool m_possible_change_data_location;
        QString m_encrypted_password_key;
        QString m_color_panel_background_border;
        QString m_color_panel_background;
        QString m_color_button_background_over;
        QString m_color_button_background;
        QString m_color_text_on_background;
        QString m_color_text_gray;
        QString m_color_disabled;
        QString m_border_window;
};

#endif // SETTINGS__RESOURCESSETTINGS_H
