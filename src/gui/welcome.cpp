#include "gui/welcome.h"
#include "ui_welcome.h"
#include "style/stylemanager.h"
#include <QDebug>
#include <QPropertyAnimation>

Welcome::Welcome(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Welcome)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    connect(ui->buttonSimpleInstallation, SIGNAL(clicked()), this, SLOT(simpleInstallationEvent()));
    connect(ui->buttonCustomInstall, SIGNAL(clicked()), this, SLOT(customInstallationEvent()));
    connect(ui->buttonContract, SIGNAL(clicked()), this, SLOT(customContractEvent()));
}

Welcome::~Welcome()
{
    delete ui;
}

void Welcome::changeLanguage()
{
    ui->retranslateUi(this);
}

void Welcome::customInstallationEvent()
{
    customInstallationSignal();
}

void Welcome::simpleInstallationEvent()
{
    simpleInstallationSignal();
}

void Welcome::customContractEvent()
{
    contractSignal();
}

// dans slot -> signal
