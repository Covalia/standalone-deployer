#include "gui/manager/uimanager.h"

#include <QApplication>

#include "log/logger.h"
#include "gui/forms/windowui.h"
#include "gui/forms/installationui.h"
#include "gui/forms/endinstallationui.h"
#include "gui/wizard/installwizard.h"

UIManager::UIManager(const QString &_appName, bool _changeDataLocationAllowed) : QObject(),
    m_window(nullptr),
    m_installation(nullptr),
    m_endInstallation(nullptr),
    m_wizard(nullptr),
    m_currentWidget(nullptr)
{
    m_installation = new InstallationUI;
    m_endInstallation = new EndInstallationUI;

    m_wizard = new InstallWizard(_appName, _changeDataLocationAllowed);
    m_wizard->retranslateUi();

    m_window = new WindowUI(m_wizard, _appName);
    m_currentWidget = m_wizard;

    connect(m_wizard, SIGNAL(finished(int)), this, SLOT(wizardFinished(int)));
    connect(m_window, SIGNAL(changeLanguageSignal(const QString&)),
            this, SLOT(languageChanged(const QString&)));
    connect(m_endInstallation, SIGNAL(quitInstaller(bool)), this, SIGNAL(quitInstaller(bool)));

    m_window->show();
    m_window->center();
}

UIManager::~UIManager()
{
    // remove m_window ownership in case wizard is currently running.
    m_wizard->setParent(nullptr);

    delete m_window;
    delete m_wizard;
    // m_installation, m_endInstallation are children of m_window, deleted by m_window.
}

void UIManager::init(const QString &_locale)
{
    m_window->setLocale(_locale);
    m_wizard->show();
}

bool UIManager::isCustomInstallation() const
{
    return m_wizard->isCustomInstallation();
}

QString UIManager::getInstallationFolder() const
{
    return m_wizard->getInstallationFolder();
}

bool UIManager::isDataFolderChosen() const
{
    return m_wizard->isDataFolderChosen();
}

void UIManager::setDataFolderChosen(bool _folderChosen)
{
    m_wizard->setDataFolderChosen(_folderChosen);
}

QString UIManager::getDataFolder() const
{
    return m_wizard->getDataFolder();
}

bool UIManager::isCreatedOfflineShortcut() const
{
    return m_wizard->isCreatedOfflineShortcut();
}

bool UIManager::isLaunchedAppAtStartUp() const
{
    return m_wizard->isLaunchedAppAtStartUp();
}

bool UIManager::isProxyUsed() const
{
    return m_wizard->isProxyUsed();
}

QString UIManager::getProxyHostname() const
{
    return m_wizard->getProxyHostname();
}

quint16 UIManager::getProxyPort() const
{
    return m_wizard->getProxyPort();
}

QString UIManager::getProxyLogin() const
{
    return m_wizard->getProxyLogin();
}

QString UIManager::getProxyPassword() const
{
    return m_wizard->getProxyPassword();
}

bool UIManager::isStartedAppWhenInstalled() const
{
    return m_wizard->isStartedAppWhenInstalled();
}

void UIManager::setInstallationFolder(const QString &_installationFolder)
{
    m_wizard->setInstallationFolder(_installationFolder);
}

void UIManager::setDataFolder(const QString &_dataFolder)
{
    m_wizard->setDataFolder(_dataFolder);
}

void UIManager::setCreatedOfflineShortcut(const bool _createdOfflineShortcut)
{
    m_wizard->setCreatedOfflineShortcut(_createdOfflineShortcut);
}

void UIManager::setLaunchedAppAtStartUp(const bool _launchedAppAtStartUp)
{
    m_wizard->setLaunchedAppAtStartUp(_launchedAppAtStartUp);
}

void UIManager::setStartedAppWhenInstalled(const bool _startedAppWhenInstalled)
{
    m_wizard->setStartedAppWhenInstalled(_startedAppWhenInstalled);
}

void UIManager::wizardFinished(int _result)
{
    Q_UNUSED(_result)

    // remove m_window ownership to be able to later access wizard properties.
    m_wizard->setParent(nullptr);

    m_installation->retranslateUi();
    m_window->setCentralWidget(m_installation);
    m_currentWidget = m_installation;
    emit wizardFinishedSignal();
}

void UIManager::languageChanged(const QString &_language)
{
    Q_UNUSED(_language)

    m_currentWidget->retranslateUi();
    m_window->retranslateUi();
}

void UIManager::eventEndInstallation(bool _success, QStringList _errors)
{
    m_installation->disconnect();
    if (!_success) {
        m_endInstallation->showErrors(_errors);
    } else {
        // indicate if application must start when installer will be closed
        m_endInstallation->setStartedAppWhenInstalled(m_wizard->isStartedAppWhenInstalled());
    }
    m_endInstallation->retranslateUi();

    m_window->setCentralWidget(m_endInstallation);
    m_currentWidget = m_endInstallation;
}

void UIManager::setProxyUsed(bool _proxyUsed)
{
    m_wizard->setProxyUsed(_proxyUsed);
}

void UIManager::setProxyHostname(const QString &_hostname)
{
    m_wizard->setProxyHostname(_hostname);
}

void UIManager::setProxyPort(quint16 _port)
{
    m_wizard->setProxyPort(_port);
}

void UIManager::setProxyLogin(const QString &_login)
{
    m_wizard->setProxyLogin(_login);
}

void UIManager::setProxyPassword(const QString _password)
{
    m_wizard->setProxyPassword(_password);
}

void UIManager::setCustomInstallation(bool _customInstallation)
{
    m_wizard->setCustomInstallation(_customInstallation);
}

void UIManager::printWizard() const
{
    m_wizard->print();
}

QString UIManager::getLocale() const
{
    return m_window->getLocale();
}
