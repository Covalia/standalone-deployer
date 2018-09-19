#include "gui/forms/proxyui.h"
#include "ui_proxy.h"

#include "gui/style/stylemanager.h"
#include "settings/settings.h"

ProxyUI::ProxyUI(QSharedPointer<ResourcesSettings> _resourcesSettings, QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::ProxyUI)
{
    m_ui->setupUi(this);

    // link style
    m_ui->buttonValidateProxy->setAccessibleName("pageButton");

    StyleManager::transformStyle(_resourcesSettings, this);

    // init with actual settings
    Settings * settings = Settings::getInstance();

    m_ui->checkBoxUseProxy->setChecked(settings->isProxyUse());

    m_ui->lineEditHostname->setText(settings->getProxyHostname());
    if (settings->getProxyPort() >= 0 && settings->getProxyPort() <= 65535) {
        m_ui->spinBoxPort->setValue(settings->getProxyPort());
    }
    m_ui->lineEditLogin->setText(settings->getProxyLogin());
    m_ui->lineEditPassword->setText(settings->getProxyPassword());

    updateVisibleField();

    connect(m_ui->buttonValidateProxy, SIGNAL(clicked()), this, SLOT(validateSettings()));
    connect(m_ui->checkBoxUseProxy, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleField()));
}

ProxyUI::~ProxyUI()
{
    delete m_ui;
}

void ProxyUI::updateVisibleField()
{
    const bool enabled = m_ui->checkBoxUseProxy->isChecked();

    m_ui->labelHostname->setEnabled(enabled);
    m_ui->labelPort->setEnabled(enabled);
    m_ui->labelLogin->setEnabled(enabled);
    m_ui->labelPassword->setEnabled(enabled);

    m_ui->lineEditHostname->setEnabled(enabled);
    m_ui->spinBoxPort->setEnabled(enabled);
    m_ui->lineEditLogin->setEnabled(enabled);
    m_ui->lineEditPassword->setEnabled(enabled);
}

void ProxyUI::changeLanguage()
{
    m_ui->retranslateUi(this);
}

void ProxyUI::validateSettings()
{
    Settings * settings = Settings::getInstance();

    settings->setProxyUse(m_ui->checkBoxUseProxy->isChecked());

    settings->setProxyHostname(m_ui->lineEditHostname->text());

    settings->setProxyPort(m_ui->spinBoxPort->value());
    settings->setProxyLogin(m_ui->lineEditLogin->text());
    settings->setProxyPassword(m_ui->lineEditPassword->text());

    emit validateSettingsSignal();
}
