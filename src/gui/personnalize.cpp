#include "gui/personnalize.h"
#include "ui_personnalize.h"
#include "style/stylemanager.h"
#include "log/logger.h"
#include "settings/settings.h"

#include <QStringList>
#include <QFileDialog>

Personnalize::Personnalize(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Personnalize)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    Settings * settings = Settings::getInstance();
    ui->editLineFolderInstallation->setText(settings->getInstallLocation());
    ui->editLineDataInstallation->setText(settings->getDataLocation());
    ui->checkBoxOfflineShortcut->setChecked(settings->getShortcutOffline());
    ui->checkBoxRunAtStart->setChecked(settings->getRunAtStart());

    connect(ui->buttonProxySetting, SIGNAL(clicked()), this, SLOT(proxySettingEvent()));
    connect(ui->buttonChangeFolderInstallation, SIGNAL(clicked()), this, SLOT(fileChooserInstallEvent()));
    connect(ui->buttonChangeDataInstallation, SIGNAL(clicked()), this, SLOT(fileChooserDataEvent()));
    connect(ui->buttonStartInstallation, SIGNAL(clicked()), this, SLOT(customInstallationEvent()));
}

Personnalize::~Personnalize()
{
    delete ui;
}

void Personnalize::changeLanguage()
{
    ui->retranslateUi(this);
}

void Personnalize::proxySettingEvent()
{
    saveElementsInSetting();
    proxySettingSignal();
}

void Personnalize::fileChooserInstallEvent()
{
    QString folderPath = fileChooserDialog();

    if (!folderPath.isEmpty()) {
        ui->editLineFolderInstallation->setText(folderPath);
    }
}

void Personnalize::fileChooserDataEvent()
{
    QString dataPath = fileChooserDialog();

    if (!dataPath.isEmpty()) {
        ui->editLineDataInstallation->setText(dataPath);
    }
}

QString Personnalize::fileChooserDialog()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::DontUseNativeDialog, false);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        if (fileNames.size() > 0) {
            L_INFO("Selected file = " + fileNames.at(0));
            return fileNames.at(0);
        }
    }
    return "";
}

void Personnalize::saveElementsInSetting(){
    Settings * settings = Settings::getInstance();

    //install folder
    settings->setInstallLocation(ui->editLineFolderInstallation->text());
    //data folder
    if(ui->checkBoxDataInstallation->isVisible() && ui->checkBoxDataInstallation->isChecked()){
        settings->setDataLocation(ui->editLineDataInstallation->text());
    }
    else{
        settings->setDataLocation(ui->editLineFolderInstallation->text() + "/Data");
    }
    // offline shortcut
    if(ui->checkBoxOfflineShortcut->isVisible()){
        settings->setShortcutOffline(ui->checkBoxOfflineShortcut->isChecked());
    }
    // run at start
    if(ui->checkBoxRunAtStart->isVisible()){
        settings->setRunAtStart(ui->checkBoxRunAtStart->isChecked());
    }
}

void Personnalize::customInstallationEvent()
{
    saveElementsInSetting();

    customInstallationSignal();
}
