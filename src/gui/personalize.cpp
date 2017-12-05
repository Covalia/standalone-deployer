#include "gui/personalize.h"
#include "ui_personalize.h"
#include "style/stylemanager.h"
#include "log/logger.h"
#include "settings/settings.h"

#include <QStringList>
#include <QFileDialog>
#include <QDebug>

Personalize::Personalize(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::Personalize)
{
    m_ui->setupUi(this);

    StyleManager::transformStyle(this);

    Settings * settings = Settings::getInstance();
    m_ui->editLineFolderInstallation->setText(settings->getInstallLocation());
    m_ui->editLineDataInstallation->setText(settings->getDataLocation());
    m_ui->checkBoxOfflineShortcut->setChecked(settings->getShortcutOffline());
    m_ui->checkBoxRunAtStart->setChecked(settings->getRunAtStart());

    connect(m_ui->buttonProxySetting, SIGNAL(clicked()), this, SLOT(proxySettingEvent()));
    connect(m_ui->buttonChangeFolderInstallation, SIGNAL(clicked()), this, SLOT(fileChooserInstallEvent()));
    connect(m_ui->buttonChangeDataInstallation, SIGNAL(clicked()), this, SLOT(fileChooserDataEvent()));
    connect(m_ui->buttonStartInstallation, SIGNAL(clicked()), this, SLOT(customInstallationEvent()));
}

Personalize::~Personalize()
{
    delete m_ui;
}

void Personalize::changeLanguage()
{
    m_ui->retranslateUi(this);
}

void Personalize::proxySettingEvent()
{
    saveElementsInSetting();
    proxySettingSignal();
}

void Personalize::fileChooserInstallEvent()
{
    QString folderPath = fileChooserDialog();

    if (!folderPath.isEmpty()) {
        m_ui->editLineFolderInstallation->setText(folderPath);
    }
}

void Personalize::fileChooserDataEvent()
{
    QString dataPath = fileChooserDialog();

    if (!dataPath.isEmpty()) {
        m_ui->editLineDataInstallation->setText(dataPath);
    }
}

QString Personalize::fileChooserDialog()
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

void Personalize::saveElementsInSetting(){
    Settings * settings = Settings::getInstance();

    //install folder
    settings->setInstallLocation(m_ui->editLineFolderInstallation->text());
    //data folder
    if(m_ui->checkBoxDataInstallation->isVisible() && m_ui->checkBoxDataInstallation->isChecked()){
        settings->setDataLocation(m_ui->editLineDataInstallation->text());
    }
    else{
        settings->setDataLocation(m_ui->editLineFolderInstallation->text() + "/Data");
    }
    // offline shortcut
    if(m_ui->checkBoxOfflineShortcut->isVisible()){
        settings->setShortcutOffline(m_ui->checkBoxOfflineShortcut->isChecked());
    }
    // run at start
    if(m_ui->checkBoxRunAtStart->isVisible()){
        settings->setRunAtStart(m_ui->checkBoxRunAtStart->isChecked());
    }
}

void Personalize::customInstallationEvent()
{
    saveElementsInSetting();

    customInstallationSignal();
}
