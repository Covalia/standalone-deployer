#include "gui/manager/uimanager.h"

#include <QApplication>

#include "log/logger.h"
#include "gui/forms/windowui.h"
#include "gui/forms/welcomeui.h"
#include "gui/forms/personalizeui.h"
#include "gui/forms/proxyui.h"
#include "gui/forms/aboutui.h"
#include "gui/forms/installationui.h"
#include "gui/forms/endinstallationui.h"

UIManager::UIManager(QSharedPointer<ResourceSettings> _resourceSettings) : QObject(),
    m_resourceSettings(_resourceSettings),
    m_window(0),
    m_welcome(0),
    m_personalize(0),
    m_proxy(0),
    m_about(0),
    m_installation(0),
    m_endInstallation(0)
{
    m_window = new WindowUI(m_resourceSettings);
    m_welcome = new WelcomeUI(m_resourceSettings);
    m_personalize = new PersonalizeUI(m_resourceSettings);
    m_proxy = new ProxyUI(m_resourceSettings);
    m_about = new AboutUI(m_resourceSettings);
    m_installation = new InstallationUI(m_resourceSettings);
    m_endInstallation = new EndInstallationUI(m_resourceSettings);

    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_window, SLOT(changeLanguage()));
    connect(m_window, SIGNAL(aboutSignal()),
            this, SLOT(aboutEvent()));

    m_window->show();
    m_window->center();
}

void UIManager::init()
{
    changeWelcome();
}

UIManager::~UIManager()
{
    delete m_window;
    delete m_welcome;
    delete m_personalize;
    delete m_proxy;
    delete m_about;
    delete m_installation;
    delete m_endInstallation;
}

void UIManager::aboutEvent()
{
    changeAbout();
}

void UIManager::returnToLastPage()
{
    QWidget * widget = 0;

    switch (m_returnPage) {
        case WelcomePage:
            widget = m_welcome;
            break;
        case PersonalizePage:
            widget = m_personalize;
            break;
        case ProxyPage:
            widget = m_proxy;
            break;
        case AboutPage:
            widget = m_about;
            break;
        case InstallationPage:
            widget = m_installation;
            break;
        case EndInstallationPage:
            widget = m_endInstallation;
            break;
    } // switch

    m_window->changeContentWidget(widget);
    m_window->setVisibleButton(true, true);
}

void UIManager::changeWelcome()
{
    if (m_welcome) {
        m_window->changeContentWidget(m_welcome);
        m_returnPage = WelcomePage;
        m_window->setVisibleButton(true, true);

        connect(m_window, SIGNAL(changeLanguageSignal()),
                m_welcome, SLOT(changeLanguage()));
        connect(m_welcome, SIGNAL(customInstallationSignal()),
                this, SLOT(switchWelcomeToPersonalize()));
        connect(m_welcome, SIGNAL(contractSignal()),
                this, SLOT(aboutEvent()));
        connect(m_welcome, SIGNAL(simpleInstallationSignal()),
                this, SLOT(switchWelcomeToInstallation()), Qt::ConnectionType(Qt::QueuedConnection | Qt::UniqueConnection));
    }
}

void UIManager::changePersonalize()
{
    m_window->changeContentWidget(m_personalize);
    m_returnPage = PersonalizePage;
    m_window->setVisibleButton(true, true);

    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_personalize, SLOT(changeLanguage()));
    connect(m_personalize, SIGNAL(proxySettingSignal()),
            this, SLOT(switchPersonalizeToProxy()));
    connect(m_personalize, SIGNAL(customInstallationSignal()),
            this, SLOT(switchWelcomeToInstallation()));
    connect(m_personalize, SIGNAL(installationFolderChanged(QString)),
            this, SLOT(installationFolderChangedEvent(QString)));
}

void UIManager::changeProxy()
{
    if (m_proxy) {
        m_window->changeContentWidget(m_proxy);
        m_returnPage = ProxyPage;
        m_window->setVisibleButton(true, true);

        connect(m_window, SIGNAL(changeLanguageSignal()),
                m_proxy, SLOT(changeLanguage()));
        connect(m_proxy, SIGNAL(validateSettingsSignal()),
                this, SLOT(switchProxyToPersonalize()));
    }
}

void UIManager::changeAbout()
{
    if (m_about) {
        m_window->changeContentWidget(m_about);
        m_window->setVisibleButton(false, true);

        connect(m_window, SIGNAL(changeLanguageSignal()),
                m_about, SLOT(changeLanguage()));
        connect(m_about, SIGNAL(validateAboutSignal()),
                this, SLOT(switchAboutTo()));
    }
}

void UIManager::changeInstallation()
{
    m_window->changeContentWidget(m_installation);
    m_window->setVisibleButton(false, false);
    m_window->update();
    emit changeInstallationSignal();
}

void UIManager::changeEndInstallation()
{
    m_window->changeContentWidget(m_endInstallation);
    m_window->setVisibleButton(false, false);
    connect(m_endInstallation, SIGNAL(closeInstallationSignal(bool)),
            this, SLOT(eventCloseInstallation(bool)));
}

void UIManager::switchWelcomeToPersonalize()
{
    m_welcome->disconnect();
    changePersonalize();
}

void UIManager::switchWelcomeToInstallation()
{
    m_welcome->disconnect();
    changeInstallation();
}

void UIManager::switchPersonalizeToProxy()
{
    m_personalize->disconnect();
    changeProxy();
}

void UIManager::switchPersonalizeToInstallation()
{
    m_personalize->disconnect();
    changeInstallation();
}

void UIManager::switchProxyToPersonalize()
{
    m_proxy->disconnect();
    changePersonalize();
}

void UIManager::switchAboutTo()
{
    m_about->disconnect();
    returnToLastPage();
}

void UIManager::eventEndInstallation(bool _success, QStringList _errors)
{
    m_installation->disconnect();
    changeEndInstallation();
    if (!_success && m_endInstallation) {
        m_endInstallation->showErrors(_errors);
    }
}

void UIManager::eventCloseInstallation(bool _launchApplication)
{
    emit closeInstallationSignal(_launchApplication);
}

void UIManager::installationFolderChangedEvent(QString _folder)
{
    emit installationFolderChanged(_folder);
}
