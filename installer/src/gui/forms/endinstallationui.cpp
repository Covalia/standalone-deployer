#include "endinstallationui.h"
#include "ui_endinstallation.h"
#include "log/logger.h"

EndInstallationUI::EndInstallationUI(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::EndInstallationUI),
    m_start(false)
{
    m_ui->setupUi(this);

    connect(m_ui->buttonFinish, SIGNAL(clicked()), this, SLOT(closeInstallationEvent()));
}

EndInstallationUI::~EndInstallationUI()
{
    delete m_ui;
}

void EndInstallationUI::showErrors(QStringList _errors)
{
    m_errors = _errors;

    m_ui->labelIcon->setPixmap(QPixmap(":/images/failure.png"));
    m_ui->labelInstallationSuccess->setVisible(false);
    m_ui->labelInstallationError->setVisible(true);
    m_ui->labelListOfErrors->setVisible(true);
}

void EndInstallationUI::showSuccess()
{
    m_ui->labelIcon->setPixmap(QPixmap(":/images/success.png"));
    m_ui->labelInstallationSuccess->setVisible(true);
    m_ui->labelInstallationError->setVisible(false);
    m_ui->labelListOfErrors->setText("");
    m_ui->labelListOfErrors->setVisible(false);
}

void EndInstallationUI::setStartedAppWhenInstalled(bool _start)
{
    m_start = _start;
}

void EndInstallationUI::closeInstallationEvent()
{
    if (m_errors.isEmpty()) {
        emit quitInstaller(m_start);
    } else {
        // in case of errors
        emit quitInstaller(false);
    }
}

void EndInstallationUI::retranslateUi()
{
    L_INFO("EndInstallationUI::retranslateUi() called");

    QString errorMessage = "";
    for (QStringList::iterator it = m_errors.begin(); it != m_errors.end(); ++it) {
        QString current = *it;
        // this string is part of InstallManager.
        errorMessage += "• " + translate_helper("InstallManager", current) + "\n";
    }
    m_ui->labelListOfErrors->setText(errorMessage);

    m_ui->retranslateUi(this);
}
