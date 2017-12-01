#include "gui/proxy.h"
#include "ui_proxy.h"
#include "style/stylemanager.h"
#include "settings/settings.h"

Proxy::Proxy(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Proxy)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    QButtonGroup * group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->checkBoxProxy);
    group->addButton(ui->checkBoxProxyAuto);
    group->addButton(ui->checkBoxProxyManual);

    // init with actual settings
    Settings * settings = Settings::getInstance();
    if (!settings->getProxyUse()) {
        ui->checkBoxProxy->setChecked(true);
    } else if (settings->getProxyAuto()) {
        ui->checkBoxProxyAuto->setChecked(true);
    } else if (settings->getProxyManual()) {
        ui->checkBoxProxyManual->setChecked(true);
    }
    ui->lineEditURL->setText(settings->getProxyURL());
    if (settings->getProxyPort() > 0) {
        ui->lineEditPort->setText(QString::number(settings->getProxyPort()));
    }
    ui->lineEditLogin->setText(settings->getProxyLogin());
    ui->lineEditPassword->setText(settings->getProxyPassword());

    updateVisibleField();

    connect(ui->buttonValidateProxy, SIGNAL(clicked()), this, SLOT(validateSettings()));
    connect(ui->checkBoxProxy, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleField()));
    connect(ui->checkBoxProxyAuto, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleField()));
    connect(ui->checkBoxProxyManual, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleField()));
}

Proxy::~Proxy()
{
    delete ui;
}

void Proxy::updateVisibleField()
{
    ui->labelURL->setEnabled(ui->checkBoxProxyManual->isChecked());
    ui->labelPort->setEnabled(ui->checkBoxProxyManual->isChecked());
    ui->labelLogin->setEnabled(ui->checkBoxProxyManual->isChecked());
    ui->labelPassword->setEnabled(ui->checkBoxProxyManual->isChecked());

    ui->lineEditURL->setEnabled(ui->checkBoxProxyManual->isChecked());
    ui->lineEditPort->setEnabled(ui->checkBoxProxyManual->isChecked());
    ui->lineEditLogin->setEnabled(ui->checkBoxProxyManual->isChecked());
    ui->lineEditPassword->setEnabled(ui->checkBoxProxyManual->isChecked());
}

void Proxy::changeLanguage()
{
    ui->retranslateUi(this);
}

void Proxy::validateSettings()
{
    Settings * settings = Settings::getInstance();

    settings->setProxyUse(!ui->checkBoxProxy->isChecked());
    settings->setProxyAuto(ui->checkBoxProxyAuto->isChecked());
    settings->setProxyManual(ui->checkBoxProxyManual->isChecked());

    settings->setProxyURL(ui->lineEditURL->text());

    // port parsing to int
    if (!ui->lineEditPort->text().isEmpty()) {
        bool okParsePort = false;
        int port = ui->lineEditPort->text().toInt(&okParsePort, 10);
        if (okParsePort) {
            settings->setProxyPort(port);
        }
    }
    settings->setProxyLogin(ui->lineEditLogin->text());
    settings->setProxyPassword(ui->lineEditPassword->text());

    validateSettingsSignal();
} // Proxy::validateSettings
