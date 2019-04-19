#include <QtWidgets>
#include <QtDebug>
#include "gui/wizard/installwizard.h"
#include "gui/wizard/pages/welcome.h"
#include "gui/wizard/pages/license.h"
#include "gui/wizard/pages/type.h"
#include "gui/wizard/pages/folders.h"
#include "gui/wizard/pages/shortcuts.h"
#include "gui/wizard/pages/proxy.h"
#include "gui/wizard/pages/launch.h"
#include "gui/wizard/pages/thirdparty.h"
#include "log/logger.h"

InstallWizard::InstallWizard(const QString &_appName, bool _changeDataLocationAllowed, QWidget * parent) : QWizard(parent),
    m_nextButtonText(QT_TR_NOOP("Next >")),
    m_backButtonText(QT_TR_NOOP("< Back")),
    m_finishButtonText(QT_TR_NOOP("Install"))
{
    setPage(Page_Welcome, new WelcomePage(_appName));
    setPage(Page_License, new LicensePage);
    setPage(Page_Type, new TypePage);
    setPage(Page_Folders, new FoldersPage(_changeDataLocationAllowed));
    setPage(Page_Shortcuts, new ShortcutsPage);
    setPage(Page_Proxy, new ProxyPage);
    setPage(Page_Launch, new LaunchPage);
    setPage(Page_ThirdParty, new ThirdPartyPage);

    setStartId(Page_Welcome);
    setOption(QWizard::IndependentPages, true);
    setOption(QWizard::NoBackButtonOnStartPage, true);

    setWizardStyle(ModernStyle);

    setButtonText(QWizard::NextButton, tr_helper(m_nextButtonText));
    setButtonText(QWizard::BackButton, tr_helper(m_backButtonText));
    setButtonText(QWizard::FinishButton, tr_helper(m_finishButtonText));
}

bool InstallWizard::isCustomInstallation() const
{
    return field("type.custom").toBool();
}

void InstallWizard::setCustomInstallation(bool _customInstallation)
{
    if (_customInstallation) {
        setField("type.custom", true);
        setField("type.simple", false);
    } else {
        setField("type.simple", true);
        setField("type.custom", false);
    }
}

QString InstallWizard::getInstallationFolder() const
{
    if (isCustomInstallation()) {
        return field("folders.installationFolder").toString();
    } else {
        return "";
    }
}

bool InstallWizard::isDataFolderChosen() const
{
    if (isCustomInstallation()) {
        return field("folders.chooseDataFolder").toBool();
    } else {
        return false;
    }
}

void InstallWizard::setDataFolderChosen(bool _folderChosen)
{
    setField("folders.chooseDataFolder", _folderChosen);
}

QString InstallWizard::getDataFolder() const
{
    if (isCustomInstallation()) {
        return field("folders.dataFolder").toString();
    } else {
        return "";
    }
}

bool InstallWizard::isCreatedOfflineShortcut() const
{
    if (isCustomInstallation()) {
        return field("shortcuts.createOfflineShortcut").toBool();
    } else {
        return false;
    }
}

bool InstallWizard::isLaunchedAppAtStartUp() const
{
    if (isCustomInstallation()) {
        return field("shortcuts.launchAppAtStartUp").toBool();
    } else {
        return false;
    }
}

bool InstallWizard::isProxyUsed() const
{
    if (isCustomInstallation()) {
        return field("proxy.use").toBool();
    } else {
        return false;
    }
}

QString InstallWizard::getProxyHostname() const
{
    if (isCustomInstallation()) {
        return field("proxy.hostname").toString();
    } else {
        return "";
    }
}

quint16 InstallWizard::getProxyPort() const
{
    if (isCustomInstallation()) {
        return static_cast<quint16>(field("proxy.port").toUInt());
    } else {
        return 1;
    }
}

QString InstallWizard::getProxyLogin() const
{
    if (isCustomInstallation()) {
        return field("proxy.login").toString();
    } else {
        return "";
    }
}

QString InstallWizard::getProxyPassword() const
{
    if (isCustomInstallation()) {
        return field("proxy.password").toString();
    } else {
        return "";
    }
}

bool InstallWizard::isStartedAppWhenInstalled() const
{
    return field("launch.startAppWhenInstalled").toBool();
}

void InstallWizard::setInstallationFolder(const QString &_installationFolder)
{
    setField("folders.installationFolder", _installationFolder);
}

void InstallWizard::setDataFolder(const QString &_dataFolder)
{
    setField("folders.dataFolder", _dataFolder);
}

void InstallWizard::setCreatedOfflineShortcut(const bool _createdOfflineShortcut)
{
    setField("shortcuts.createOfflineShortcut", _createdOfflineShortcut);
}

void InstallWizard::setLaunchedAppAtStartUp(const bool _launchedAppAtStartUp)
{
    setField("shortcuts.launchAppAtStartUp", _launchedAppAtStartUp);
}

void InstallWizard::setStartedAppWhenInstalled(const bool _startedAppWhenInstalled)
{
    setField("launch.startAppWhenInstalled", _startedAppWhenInstalled);
}

void InstallWizard::keyPressEvent(QKeyEvent * _event)
{
    // we disable wizard closing by escape key.
    if (_event->key() != Qt::Key_Escape) {
        QWizard::keyPressEvent(_event);
    }
}

void InstallWizard::retranslateUi()
{
    L_INFO("InstallWizard::retranslateUi() called");

    const QString className = metaObject()->className();

    setButtonText(QWizard::NextButton, translate_helper(className, m_nextButtonText));
    setButtonText(QWizard::BackButton, translate_helper(className, m_backButtonText));
    setButtonText(QWizard::FinishButton, translate_helper(className, m_finishButtonText));

    static_cast<WelcomePage *>(page(Page_Welcome))->retranslateUi();
    static_cast<LicensePage *>(page(Page_License))->retranslateUi();
    static_cast<ThirdPartyPage *>(page(Page_ThirdParty))->retranslateUi();
    static_cast<TypePage *>(page(Page_Type))->retranslateUi();
    static_cast<FoldersPage *>(page(Page_Folders))->retranslateUi();
    static_cast<ShortcutsPage *>(page(Page_Shortcuts))->retranslateUi();
    static_cast<ProxyPage *>(page(Page_Proxy))->retranslateUi();
    static_cast<LaunchPage *>(page(Page_Launch))->retranslateUi();
}

void InstallWizard::setProxyUsed(bool _proxyUsed)
{
    setField("proxy.use", _proxyUsed);
}

void InstallWizard::setProxyHostname(const QString &_hostname)
{
    setField("proxy.hostname", _hostname);
}

void InstallWizard::setProxyPort(quint16 _port)
{
    setField("proxy.port", _port);
}

void InstallWizard::setProxyLogin(const QString &_login)
{
    setField("proxy.login", _login);
}

void InstallWizard::setProxyPassword(const QString _password)
{
    setField("proxy.password", _password);
}

void InstallWizard::print() const
{
    qDebug() << "----------------------------------------";
    qDebug() << "isCustomInstallation(): " << QString(isCustomInstallation() ? "yes" : "no");
    qDebug() << "getInstallationFolder(): " << getInstallationFolder();
    qDebug() << "isDataFolderChosen(): " << QString(isDataFolderChosen() ? "yes" : "no");
    qDebug() << "getDataFolder(): " << getDataFolder();
    qDebug() << "isStartedAppWhenInstalled(): " << QString(isStartedAppWhenInstalled() ? "yes" : "no");
    qDebug() << "isProxyUsed(): " << QString(isProxyUsed() ? "yes" : "no");
    qDebug() << "getProxyHostname(): " << getProxyHostname();
    qDebug() << "getProxyPort(): " << QString::number(getProxyPort());
    qDebug() << "getProxyLogin(): " << getProxyLogin();
    qDebug() << "getProxyPassword(): " << getProxyPassword();
    qDebug() << "isLaunchedAppAtStartUp(): " << QString(isLaunchedAppAtStartUp() ? "yes" : "no");
    qDebug() << "isCreatedOfflineShortcut(): " << QString(isCreatedOfflineShortcut() ? "yes" : "no");
    qDebug() << "----------------------------------------";
}
