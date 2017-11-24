#include "uimanager.h"

#include "log/simpleqtlogger.h"
#include "gui/windows.h"
#include "gui/welcome.h"
#include "gui/personnalize.h"
#include "gui/proxy.h"
#include "gui/about.h"
#include "gui/downloadui.h"

UIManager::UIManager() : QObject(),
    m_window(0),
    m_welcome(0),
    m_personnalize(0),
    m_proxy(0),
    m_about(0),
    m_download(0),
    m_saveWidget(0){
    m_window = new Windows();
    m_window->show();
    m_window->center();

    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_window, SLOT(changeLanguage()));

    connect(m_window, SIGNAL(aboutSignal()),
            this, SLOT(aboutEvent()));
}

UIManager::~UIManager(){
    delete m_saveWidget;
    delete m_window;
    delete m_welcome;
    delete m_personnalize;
    delete m_proxy;
    delete m_about;
    delete m_download;
}

void UIManager::aboutEvent(){
    changeAbout();
}

void UIManager::changeTo(QWidget * widget) {
    m_window->changeContentWidget(widget);
    m_window->setVisibleButton(true, true);
}

void UIManager::changeWelcome() {
    m_welcome = new Welcome(m_window);
    m_window->changeContentWidget(m_welcome);
    m_saveWidget = m_welcome;
    m_window->setVisibleButton(true, true);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_welcome, SLOT(changeLanguage()));

    connect(m_welcome, SIGNAL(customInstallationSignal()),
            this, SLOT(switchWelcomToPersonnalize()));
    connect(m_welcome, SIGNAL(contractSignal()),
            this, SLOT(aboutEvent()));
    connect(m_welcome, SIGNAL(simpleInstallationSignal()),
            this, SLOT(switchWelcomToDownload()));
}

void UIManager::changePersonnalize(){
    m_personnalize = new Personnalize(m_window);
    m_window->changeContentWidget(m_personnalize);
    m_saveWidget = m_personnalize;
    m_window->setVisibleButton(true, true);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_personnalize, SLOT(changeLanguage()));

    connect(m_personnalize, SIGNAL(proxySettingSignal()),
            this, SLOT(switchPersonnalizeToProxy()));
    connect(m_personnalize, SIGNAL(customInstallationSignal()),
            this, SLOT(switchWelcomToDownload()));
}

void UIManager::changeProxy(){
    m_proxy = new Proxy(m_window);
    m_window->changeContentWidget(m_proxy);
    m_saveWidget = m_proxy;
    m_window->setVisibleButton(true, true);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_proxy, SLOT(changeLanguage()));

    connect(m_proxy, SIGNAL(validateSettingsSignal()),
            this, SLOT(switchProxyToPersonnalize()));
}

void UIManager::changeAbout(){
    m_about = new About(m_window);
    m_window->changeContentWidget(m_about, false);
    m_window->setVisibleButton(false, true);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_about, SLOT(changeLanguage()));

    connect(m_about, SIGNAL(validateAboutSignal()),
            this, SLOT(switchAboutTo()));
}

void UIManager::changeDownload(){
    m_download = new DownloadUI(m_window);
    m_window->changeContentWidget(m_download, false);
    m_window->setVisibleButton(false, false);
    connect(m_window, SIGNAL(changeLanguageSignal()),
            m_download, SLOT(changeLanguage()));
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

void UIManager::removeAbout(){
    m_about->disconnect();
    delete m_about;
}

void UIManager::removeDownload(){
    m_download->disconnect();
    delete m_download;
}

void UIManager::switchWelcomToPersonnalize() {
    removeWelcome();
    changePersonnalize();
}

void UIManager::switchWelcomToDownload() {
    removeWelcome();
    changeDownload();
}

void UIManager::switchPersonnalizeToProxy() {
    removePersonnalize();
    changeProxy();
}

void UIManager::switchPersonnalizeToDownload() {
    removePersonnalize();
    changeDownload();
}

void UIManager::switchProxyToPersonnalize() {
    removeProxy();
    changePersonnalize();
}

void UIManager::switchAboutTo() {
    removeAbout();
    changeTo(m_saveWidget);
}
