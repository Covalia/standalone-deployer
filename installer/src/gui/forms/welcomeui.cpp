#include "gui/forms/welcomeui.h"
#include "ui_welcome.h"

#include "gui/style/stylemanager.h"

#include <QDebug>

WelcomeUI::WelcomeUI(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::WelcomeUI)
{
    m_ui->setupUi(this);

    // link style
    m_ui->buttonSimpleInstallation->setAccessibleName("pageButton");

    StyleManager::transformStyle(this);

    connect(m_ui->buttonSimpleInstallation, SIGNAL(clicked()), this, SLOT(simpleInstallationEvent()));
    connect(m_ui->buttonCustomInstall, SIGNAL(clicked()), this, SLOT(customInstallationEvent()));
    connect(m_ui->buttonContract, SIGNAL(clicked()), this, SLOT(customContractEvent()));
}

WelcomeUI::~WelcomeUI()
{
    delete m_ui;
}

void WelcomeUI::changeLanguage()
{
    m_ui->retranslateUi(this);
}

void WelcomeUI::customInstallationEvent()
{
    emit customInstallationSignal();
}

void WelcomeUI::simpleInstallationEvent()
{
    emit simpleInstallationSignal();
}

void WelcomeUI::customContractEvent()
{
    emit contractSignal();
}

