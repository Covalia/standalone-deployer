#include "settings/resourcesettings.h"
#include "log/logger.h"
#include "io/config.h"

#include <QSettings>
#include <QStandardPaths>

const QString ResourceSettings::DeploymentUrl("deployment_url");
const QString ResourceSettings::AppName("app_name");
const QString ResourceSettings::DefaultLocale("default_locale");
const QString ResourceSettings::ShortcutName("shortcut_name");
const QString ResourceSettings::RunAtStart("run_at_start");
const QString ResourceSettings::DefaultInstallationPath("default_installation_path");
const QString ResourceSettings::ChangeDataLocationAllowed("change_data_location_allowed");
const QString ResourceSettings::Style("style");

ResourceSettings::ResourceSettings(const QString &_appPath) :
    m_settings(_appPath, QSettings::IniFormat),
    m_deploymentUrl(""),
    m_appName("Application"),
    m_defaultLocale(IOConfig::LocaleEnUs),
    m_shortcutName("Application"),
    m_runAtStart(false),
    m_defaultInstallationPath("$HOME"),
    m_changeDataLocationAllowed(false),
    m_style("default")
{
    L_INFO("Initialise ResourceSettings");
    m_settings.setFallbacksEnabled(false);
}

ResourceSettings::~ResourceSettings()
{
}

void ResourceSettings::readSettings()
{
    L_INFO("Read resource settings");
    m_deploymentUrl = m_settings.value(DeploymentUrl, m_deploymentUrl).toString();
    m_appName = m_settings.value(AppName, m_appName).toString();
    m_defaultLocale = m_settings.value(DefaultLocale, m_defaultLocale).toString();

    m_shortcutName = m_settings.value(ShortcutName, m_shortcutName).toString();

    m_runAtStart = m_settings.value(RunAtStart, m_runAtStart).toBool();

    m_defaultInstallationPath = getTransformedVariablePath(m_settings.value(DefaultInstallationPath, m_defaultInstallationPath).toString());

    m_changeDataLocationAllowed = m_settings.value(ChangeDataLocationAllowed, m_changeDataLocationAllowed).toBool();

    m_style = m_settings.value(Style, m_style).toString();
}

QString ResourceSettings::getTransformedVariablePath(QString _path)
{
    _path.replace(QString("$HOME"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
    _path.replace(QString("$INSTALL_PATH"), m_defaultInstallationPath);
    return _path;
}

bool ResourceSettings::isRunAtStart() const
{
    return m_runAtStart;
}

QString ResourceSettings::getDeploymentUrl() const
{
    return m_deploymentUrl;
}

QString ResourceSettings::getStyle() const
{
    return m_style;
}

QString ResourceSettings::getAppName() const
{
    return m_appName;
}

QString ResourceSettings::getDefaultLocale() const
{
    return m_defaultLocale;
}

QString ResourceSettings::getShortcutName() const
{
    return m_shortcutName;
}

QString ResourceSettings::getDefaultInstallationPath() const
{
    return m_defaultInstallationPath;
}

bool ResourceSettings::isChangeDataLocationAllowed() const
{
    return m_changeDataLocationAllowed;
}
