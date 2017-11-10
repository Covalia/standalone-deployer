#include "settings.h"

#include <QMutexLocker>

Settings Settings::m_instance = Settings();
QMutex Settings::sm_instanceMutex;
QMutex Settings::sm_settingsMutex;

Settings::Settings() {
    QString installPath("P:/DEV/Workspace_downloader/downloader/test.ini");

    m_settings = new QSettings(installPath, QSettings::IniFormat);

    m_isProxyUsing = false;
    m_proxyAuto = false;
    m_proxyURL = "";
    m_proxyPort = 0;
    m_proxyAuthentification = false;
    m_proxyLogin = "";
    m_proxyPassword = "";

    m_language = "";

    m_shortcutName = "";
    m_shortcutAllUser = false;

    m_classpathExtension = "";

    m_dataLocation = "";

    m_serverURL = "";
}

Settings::~Settings(){
    delete m_settings;
}

Settings& Settings::Instance(){
    return m_instance;
}

void Settings::putSetting(QString key, QVariant value){
    m_settings->setValue(key, value);
}

QVariant Settings::getSetting(QString key){
    return m_settings->value(key);
}

QVariant Settings::getSetting(QString key, QVariant defaultValue){
    return m_settings->value(key, defaultValue);
}

void Settings::removeSetting(QString key){
    m_settings->remove(key);
}


void Settings::writeSettings(){
    QMutexLocker locker(&sm_settingsMutex);

    m_settings->beginGroup(GROUP_PROXY);
    putSetting(S_PROXY_USING, m_isProxyUsing);
    putSetting(S_PROXY_AUTO, m_proxyAuto);
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
    putSetting(S_SHORTCUT_ALL_USER, m_shortcutAllUser);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_CLASSPATH);
    putSetting(S_CLASSPATH_EXTENSION, m_classpathExtension);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_DATA);
    putSetting(S_DATA_LOCATION, m_dataLocation);
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_SERVER);
    putSetting(S_SERVER_URL, m_serverURL);
    m_settings->endGroup();
}

void Settings::readSettings(){
    QMutexLocker locker(&sm_settingsMutex);

    m_settings->beginGroup(GROUP_PROXY);
    m_isProxyUsing = getSetting(S_PROXY_USING).toBool();
    m_proxyAuto = getSetting(S_PROXY_AUTO).toBool();
    m_proxyURL = getSetting(S_PROXY_URL).toString();
    m_proxyPort = getSetting(S_PROXY_PORT).toInt();
    m_proxyAuthentification = getSetting(S_PROXY_AUTHENTICATION).toBool();
    m_proxyLogin = getSetting(S_PROXY_LOGIN).toString();
    m_proxyPassword = getSetting(S_PROXY_PASSWORD).toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_LANGUAGE);
    m_language = getSetting(S_LANGUAGE_LANGUAGE).toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_SHORTCUT);
    m_shortcutName = getSetting(S_SHORTCUT_NAME).toString();
    m_shortcutAllUser = getSetting(S_SHORTCUT_ALL_USER).toBool();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_CLASSPATH);
    m_classpathExtension = getSetting(S_CLASSPATH_EXTENSION).toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_DATA);
    m_dataLocation = getSetting(S_DATA_LOCATION).toString();
    m_settings->endGroup();

    m_settings->beginGroup(GROUP_SERVER);
    m_serverURL = getSetting(S_SERVER_URL).toString();
    m_settings->endGroup();
}

