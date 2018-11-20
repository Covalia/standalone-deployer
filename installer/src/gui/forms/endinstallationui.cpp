#include "endinstallationui.h"
#include "ui_endinstallation.h"

EndInstallationUI::EndInstallationUI(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::EndInstallationUI),
    m_start(false)
{
    m_ui->setupUi(this);

    m_ui->labelError->setVisible(false);

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
    m_ui->labelTitle->setText(tr("An error occurred during installation"));
    m_ui->labelError->setVisible(true);

    retranslateUi();
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
    if (!m_errors.isEmpty()) {
        QString errorMessage = "";
        for (QStringList::iterator it = m_errors.begin(); it != m_errors.end(); ++it) {
            QString current = *it;
            // this string is part of InstallManager.
            errorMessage += "• " + translate_helper("InstallManager", current) + "\n";
        }
        m_ui->labelError->setText(errorMessage);
    }

    m_ui->retranslateUi(this);
}
