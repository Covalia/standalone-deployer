#include "personnalize.h"
#include "ui_personnalize.h"
#include "src/style/stylemanager.h"
#include "src/log/simpleqtlogger.h"

#include <QStringList>
#include <QFileDialog>

Personnalize::Personnalize(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Personnalize){
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    connect(ui->buttonProxySetting, SIGNAL(clicked()), this, SLOT(proxySettingEvent()));
    connect(ui->buttonChangeFolderInstallation, SIGNAL(clicked()), this, SLOT(fileChooserInstallEvent()));
    connect(ui->buttonChangeDataInstallation, SIGNAL(clicked()), this, SLOT(fileChooserDataEvent()));
}

Personnalize::~Personnalize(){
    delete ui;
}

void Personnalize::changeLanguage(){
    ui->retranslateUi(this);
}

void Personnalize::proxySettingEvent() {
    proxySettingSignal();
}

void Personnalize::fileChooserInstallEvent() {
    QString opendFolder = ui->editLineFolderInstallation->text();
    QString folderPath = fileChooserDialog(opendFolder);
    if(!folderPath.isEmpty()){
        ui->editLineFolderInstallation->setText(folderPath);
    }
}

void Personnalize::fileChooserDataEvent() {
    QString opendFolder = ui->editLineDataInstallation->text();
    QString folderPath = fileChooserDialog(opendFolder);
    if(!folderPath.isEmpty()){
        ui->editLineDataInstallation->setText(folderPath);
    }
}

 QString Personnalize::fileChooserDialog(QString openFolder){
     QFileDialog dialog(this);
     dialog.setFileMode(QFileDialog::Directory);
     dialog.setOption(QFileDialog::DontUseNativeDialog, false);
     QStringList fileNames;
     if (dialog.exec()){
         fileNames = dialog.selectedFiles();
         if(fileNames.size() > 0){
             L_INFO("Selected file = " + fileNames.at(0));
             return fileNames.at(0);
         }
     }
     return "";
 }

