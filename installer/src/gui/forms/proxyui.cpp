#include "gui/forms/proxyui.h"
#include "ui_proxy.h"

#include "gui/style/stylemanager.h"
#include "settings/settings.h"

ProxyUI::ProxyUI(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::ProxyUI)
{
    m_ui->setupUi(this);

    // link style
    m_ui->buttonValidateProxy->setAccessibleName("pageButton");

    StyleManager::transformStyle(this);

    QButtonGroup * group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(m_ui->checkBoxProxy);
    group->addButton(m_ui->checkBoxProxyAuto);
    group->addButton(m_ui->checkBoxProxyManual);

    // init with actual settings
    Settings * settings = Settings::getInstance();
    if (!settings->getProxyUse()) {
        m_ui->checkBoxProxy->setChecked(true);
    } else if (settings->getProxyAuto()) {
        m_ui->checkBoxProxyAuto->setChecked(true);
    } else if (settings->getProxyManual()) {
        m_ui->checkBoxProxyManual->setChecked(true);
    }
    m_ui->lineEditHostname->setText(settings->getProxyHostname());
    if (settings->getProxyPort() > 0) {
        m_ui->lineEditPort->setText(QString::number(settings->getProxyPort()));
    }
    m_ui->lineEditLogin->setText(settings->getProxyLogin());
    m_ui->lineEditPassword->setText(settings->getProxyPassword());

    updateVisibleField();

    connect(m_ui->buttonValidateProxy, SIGNAL(clicked()), this, SLOT(validateSettings()));
    connect(m_ui->checkBoxProxy, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleField()));
    connect(m_ui->checkBoxProxyAuto, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleField()));
    connect(m_ui->checkBoxProxyManual, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleField()));
}

ProxyUI::~ProxyUI()
{
    delete m_ui;
}

void ProxyUI::updateVisibleField()
{
    m_ui->labelHostname->setEnabled(m_ui->checkBoxProxyManual->isChecked());
    m_ui->labelPort->setEnabled(m_ui->checkBoxProxyManual->isChecked());
    m_ui->labelLogin->setEnabled(m_ui->checkBoxProxyManual->isChecked());
    m_ui->labelPassword->setEnabled(m_ui->checkBoxProxyManual->isChecked());

    m_ui->lineEditHostname->setEnabled(m_ui->checkBoxProxyManual->isChecked());
    m_ui->lineEditPort->setEnabled(m_ui->checkBoxProxyManual->isChecked());
    m_ui->lineEditLogin->setEnabled(m_ui->checkBoxProxyManual->isChecked());
    m_ui->lineEditPassword->setEnabled(m_ui->checkBoxProxyManual->isChecked());
}

void ProxyUI::changeLanguage()
{
    m_ui->retranslateUi(this);
}

void ProxyUI::validateSettings()
{
    Settings * settings = Settings::getInstance();

    settings->setProxyUse(!m_ui->checkBoxProxy->isChecked());
    settings->setProxyAuto(m_ui->checkBoxProxyAuto->isChecked());
    settings->setProxyManual(m_ui->checkBoxProxyManual->isChecked());

    settings->setProxyHostname(m_ui->lineEditHostname->text());

    // port parsing to int
    if (!m_ui->lineEditPort->text().isEmpty()) {
        bool okParsePort = false;
        int port = m_ui->lineEditPort->text().toInt(&okParsePort, 10);
        if (okParsePort) {
            settings->setProxyPort(port);
        }
    }
    settings->setProxyLogin(m_ui->lineEditLogin->text());
    settings->setProxyPassword(m_ui->lineEditPassword->text());

    validateSettingsSignal();
} // Proxy::validateSettings
