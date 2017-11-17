#include "uimanager.h"

#include "log/simpleqtlogger.h"
#include "gui/windows.h"
#include "gui/welcome.h"
#include "gui/personnalize.h"
#include "gui/proxy.h"
#include "gui/about.h"

UIManager::UIManager() : QObject(),
    m_window(0),
    m_welcome(0),
    m_personnalize(0),
    m_proxy(0),
    m_about(0){
    m_window = new Windows();
    m_window->show();
    m_window->center();

    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_window, SLOT(changeLanguage()));

    connect(m_window, SIGNAL(aboutSignal()),
            this, SLOT(aboutEvent()));
}

UIManager::~UIManager(){
    delete m_window;
    delete m_welcome;
    delete m_personnalize;
    delete m_proxy;
    delete m_about;
}


void UIManager::aboutEvent(){
    changeAbout();
}

void UIManager::changeWelcome() {
    m_welcome = new Welcome(m_window);
    m_window->changeContentWidget(m_welcome);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_welcome, SLOT(changeLanguage()));

    connect(m_welcome, SIGNAL(customInstallationSignal()),
            this, SLOT(switchWelcomToPersonnalize()));
}

void UIManager::changePersonnalize(){
    m_personnalize = new Personnalize(m_window);
    m_window->changeContentWidget(m_personnalize);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_personnalize, SLOT(changeLanguage()));

    connect(m_personnalize, SIGNAL(proxySettingSignal()),
            this, SLOT(switchPersonnalizeToProxy()));
}

void UIManager::changeProxy(){
    m_proxy = new Proxy(m_window);
    m_window->changeContentWidget(m_proxy);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_proxy, SLOT(changeLanguage()));

    connect(m_proxy, SIGNAL(validateSettingsSignal()),
            this, SLOT(switchProxyToPersonnalize()));
}

void UIManager::changeAbout(){
    m_about = new About(m_window);
    m_window->changeContentWidget(m_about);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_about, SLOT(changeLanguage()));

//    connect(m_proxy, SIGNAL(validateSettingsSignal()),
//            this, SLOT(switchProxyToPersonnalize()));
}

void UIManager::removeWelcome() {
    m_welcome->disconnect();
    delete m_welcome;
}

void UIManager::removePersonnalize(){
    m_personnalize->disconnect();
    delete m_personnalize;
}

void UIManager::removeProxy(){
    m_proxy->disconnect();
    delete m_proxy;
}

void UIManager::switchWelcomToPersonnalize() {
    removeWelcome();
    changePersonnalize();
}

void UIManager::switchPersonnalizeToProxy() {
    removePersonnalize();
    changeProxy();
}

void UIManager::switchProxyToPersonnalize() {
    removeProxy();
    changePersonnalize();
}
