#include "gui/welcome.h"
#include "ui_welcome.h"
#include "style/stylemanager.h"
#include <QDebug>

Welcome::Welcome(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::Welcome)
{
    m_ui->setupUi(this);

    StyleManager::transformStyle(this);

    connect(m_ui->buttonSimpleInstallation, SIGNAL(clicked()), this, SLOT(simpleInstallationEvent()));
    connect(m_ui->buttonCustomInstall, SIGNAL(clicked()), this, SLOT(customInstallationEvent()));
    connect(m_ui->buttonContract, SIGNAL(clicked()), this, SLOT(customContractEvent()));
}

Welcome::~Welcome()
{
    delete m_ui;
}

void Welcome::changeLanguage()
{
    m_ui->retranslateUi(this);
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
