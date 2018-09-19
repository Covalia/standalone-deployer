#include "endinstallationui.h"
#include "ui_endinstallation.h"

#include "gui/style/stylemanager.h"

EndInstallationUI::EndInstallationUI(QSharedPointer<ResourceSettings> _resourceSettings, QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::EndInstallationUI)
{
    m_ui->setupUi(this);

    StyleManager::transformStyle(_resourceSettings, this);
    m_ui->buttonFinish->setAccessibleName("pageButton");
    m_ui->labelError->setVisible(false);

    connect(m_ui->buttonFinish, SIGNAL(clicked()), this, SLOT(closeInstallationEvent()));
}

EndInstallationUI::~EndInstallationUI()
{
    delete m_ui;
}

void EndInstallationUI::showErrors(QStringList _errors)
{
    m_ui->labelIcon->setPixmap(QPixmap(":/images/failure.png"));
    m_ui->labelTitre->setText(tr("An error occurred during installation"));
    m_ui->checkBoxLaunchAfterClose->setVisible(false);
    m_ui->checkBoxLaunchAfterClose->setChecked(false);
    m_ui->labelError->setVisible(true);
    QString errorMessage = "";
    for (QStringList::iterator it = _errors.begin(); it != _errors.end(); ++it) {
        QString current = *it;
        errorMessage += "• " + current + "\n";
    }
    m_ui->labelError->setText(errorMessage);
}

void EndInstallationUI::closeInstallationEvent()
{
    emit closeInstallationSignal(m_ui->checkBoxLaunchAfterClose->isChecked());
}
