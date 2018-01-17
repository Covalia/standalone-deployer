#include "endinstallationui.h"
#include "ui_endinstallation.h"

EndInstallationUI::EndInstallationUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EndInstallationUI)
{
    ui->setupUi(this);

    ui->buttonFinish->setAccessibleName("pageButton");

    connect(ui->buttonFinish, SIGNAL(clicked()), this, SLOT(closeInstallationEvent()));
}

EndInstallationUI::~EndInstallationUI()
{
    delete ui;
}


void EndInstallationUI::showError(QString error){
    ui->labelIcon->setPixmap(QPixmap(":/images/failure.png"));
    ui->labelTitre->setText(tr("An error occurred during installation"));
    ui->checkBoxLaunchAfterClose->setVisible(false);
    ui->checkBoxLaunchAfterClose->setChecked(false);
}


void EndInstallationUI::closeInstallationEvent(){
    closeInstallationSignal(ui->checkBoxLaunchAfterClose->isChecked());
}
