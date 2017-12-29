#include "gui/forms/personalizeui.h"
#include "ui_personalize.h"

#include "gui/style/stylemanager.h"
#include "log/logger.h"
#include "settings/resourcessettings.h"
#include "settings/settings.h"

#include <QDebug>
#include <QFileDialog>
#include <QStringList>

PersonalizeUI::PersonalizeUI(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::PersonalizeUI)
{
    m_ui->setupUi(this);

    // link style
    m_ui->buttonStartInstallation->setAccessibleName("pageButton");

    StyleManager::transformStyle(this);

    Settings * settings = Settings::getInstance();

    ResourcesSettings * resource = ResourcesSettings::getInstance();
    m_ui->widgetDataInstallation->setVisible(resource->getPossible_change_data_location());
    // apply custom data path to setting
    if(resource->getPossible_change_data_location()){
        settings->setDataLocation(resource->getDefault_data_path_custom_install());
    }

    m_ui->editLineFolderInstallation->setText(settings->getInstallLocation());
    m_ui->editLineDataInstallation->setText(settings->getDataLocation());
    m_ui->checkBoxOfflineShortcut->setChecked(settings->getShortcutOffline());
    m_ui->checkBoxRunAtStart->setChecked(settings->getRunAtStart());

    connect(m_ui->buttonProxySetting, SIGNAL(clicked()), this, SLOT(proxySettingEvent()));
    connect(m_ui->buttonChangeFolderInstallation, SIGNAL(clicked()), this, SLOT(fileChooserInstallEvent()));
    connect(m_ui->buttonChangeDataInstallation, SIGNAL(clicked()), this, SLOT(fileChooserDataEvent()));
    connect(m_ui->buttonStartInstallation, SIGNAL(clicked()), this, SLOT(customInstallationEvent()));
}

PersonalizeUI::~PersonalizeUI()
{
    delete m_ui;
}

void PersonalizeUI::changeLanguage()
{
    m_ui->retranslateUi(this);
}

void PersonalizeUI::proxySettingEvent()
{
    saveElementsInSetting();
    proxySettingSignal();
}

void PersonalizeUI::fileChooserInstallEvent()
{
    QString folderPath = fileChooserDialog(m_ui->editLineFolderInstallation->text());

    if (!folderPath.isEmpty()) {
        m_ui->editLineFolderInstallation->setText(folderPath);
    }
}

void PersonalizeUI::fileChooserDataEvent()
{
    QString dataPath = fileChooserDialog(m_ui->editLineDataInstallation->text());

    if (!dataPath.isEmpty()) {
        m_ui->editLineDataInstallation->setText(dataPath);
    }
}

QString PersonalizeUI::fileChooserDialog(QString directory)
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::DontUseNativeDialog, false);
    dialog.setDirectory(directory);
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

void PersonalizeUI::saveElementsInSetting()
{
    Settings * settings = Settings::getInstance();

    // install folder
    settings->setInstallLocation(m_ui->editLineFolderInstallation->text());
    // data folder
    if (m_ui->checkBoxDataInstallation->isVisible() && m_ui->checkBoxDataInstallation->isChecked()) {
        settings->setDataLocation(m_ui->editLineDataInstallation->text());
    } else {
        settings->setDataLocation(m_ui->editLineFolderInstallation->text() + "/Data");
    }
    // offline shortcut
    if (m_ui->checkBoxOfflineShortcut->isVisible()) {
        settings->setShortcutOffline(m_ui->checkBoxOfflineShortcut->isChecked());
    }
    // run at start
    if (m_ui->checkBoxRunAtStart->isVisible()) {
        settings->setRunAtStart(m_ui->checkBoxRunAtStart->isChecked());
    }
}

void PersonalizeUI::customInstallationEvent()
{
    saveElementsInSetting();

    customInstallationSignal();
}
