#include "endinstallationui.h"
#include "ui_endinstallation.h"

#include "gui/style/stylemanager.h"

EndInstallationUI::EndInstallationUI(QWidget * parent) :
    QWidget(parent),
    m_ui(new Ui::EndInstallationUI)
{
    m_ui->setupUi(this);

    StyleManager::transformStyle(this);
    m_ui->buttonFinish->setAccessibleName("pageButton");
    m_ui->labelError->setVisible(false);

    connect(m_ui->buttonFinish, SIGNAL(clicked()), this, SLOT(closeInstallationEvent()));
}

EndInstallationUI::~EndInstallationUI()
{
    delete m_ui;
}

void EndInstallationUI::showError(QString _error)
{
    m_ui->labelIcon->setPixmap(QPixmap(":/images/failure.png"));
    m_ui->labelTitre->setText(tr("An error occurred during installation"));
    m_ui->checkBoxLaunchAfterClose->setVisible(false);
    m_ui->checkBoxLaunchAfterClose->setChecked(false);
    m_ui->labelError->setVisible(true);
    m_ui->labelError->setText(_error);
}

void EndInstallationUI::closeInstallationEvent()
{
    closeInstallationSignal(m_ui->checkBoxLaunchAfterClose->isChecked());
}
