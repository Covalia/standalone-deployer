#include "proxy.h"
#include "ui_proxy.h"
#include "style/stylemanager.h"

Proxy::Proxy(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Proxy)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    connect(ui->buttonValidateProxy, SIGNAL(clicked()), this, SLOT(validateSettings()));
}

Proxy::~Proxy()
{
    delete ui;
}

void Proxy::changeLanguage()
{
    ui->retranslateUi(this);
}

void Proxy::validateSettings()
{
    validateSettingsSignal();
}
