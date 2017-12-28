#include "uimanager/uimanager.h"

#include "log/logger.h"
#include "gui/window.h"
#include "gui/welcome.h"
#include "gui/personalize.h"
#include "gui/proxy.h"
#include "gui/about.h"
#include "gui/downloadui.h"

UIManager::UIManager() : QObject(),
    m_window(0),
    m_welcome(0),
    m_personalize(0),
    m_proxy(0),
    m_about(0),
    m_download(0)
{
    m_window = new Window();

    m_welcome = new Welcome();
    m_personalize = new Personalize();
    m_proxy = new Proxy();
    m_about = new About();
    m_download = new DownloadUI();

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
    delete m_download;
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
        case DownloadPage:
            widget = m_download;
            break;
    }

    m_window->changeContentWidget(widget);
    m_window->setVisibleButton(true, true);
} // UIManager::returnToLastPage

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
                this, SLOT(switchWelcomeToDownload()));
    }
} // UIManager::changeWelcome

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
            this, SLOT(switchWelcomeToDownload()));
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

void UIManager::changeDownload()
{
    m_window->changeContentWidget(m_download);
    m_window->setVisibleButton(false, false);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_download, SLOT(changeLanguage()));
}

void UIManager::switchWelcomeToPersonalize()
{
    m_welcome->disconnect();
    changePersonalize();
}

void UIManager::switchWelcomeToDownload()
{
    m_welcome->disconnect();
    changeDownload();
}

void UIManager::switchPersonalizeToProxy()
{
    m_personalize->disconnect();
    changeProxy();
}

void UIManager::switchPersonalizeToDownload()
{
    m_personalize->disconnect();
    changeDownload();
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
