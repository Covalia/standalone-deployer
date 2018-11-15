#include "gui/wizard/pages/folders.h"
#include "gui/wizard/installwizard.h"

#include <QtWidgets>

FoldersPage::FoldersPage(bool _changeDataLocationAllowed, QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Customize installation folders")),
    m_installationFolderLabelText(QT_TR_NOOP("&Application installation directory")),
    m_changeInstallationFolderPushButtonText(QT_TR_NOOP("Change")),
    m_chooseDataFolderCheckBoxText(QT_TR_NOOP("&Choose a custom data directory")),
    m_changeDataFolderPushButtonText(QT_TR_NOOP("Change")),
    m_installationFolderLabel(0),
    m_installationFolderLineEdit(0),
    m_changeInstallationFolderPushButton(0),
    m_chooseDataFolderCheckBox(0),
    m_dataFolderLineEdit(0),
    m_changeDataFolderPushButton(0),
    m_changeDataLocationAllowed(_changeDataLocationAllowed)
{
    setTitle(tr_helper(m_titleText));

    m_installationFolderLabel = new QLabel(tr_helper(m_installationFolderLabelText), this);
    m_installationFolderLineEdit = new QLineEdit(this);
    m_changeInstallationFolderPushButton = new QPushButton(tr_helper(m_changeInstallationFolderPushButtonText), this);

    m_installationFolderLabel->setBuddy(m_installationFolderLineEdit);

    m_chooseDataFolderCheckBox = new QCheckBox(tr_helper(m_chooseDataFolderCheckBoxText), this);
    m_dataFolderLineEdit = new QLineEdit(this);
    m_changeDataFolderPushButton = new QPushButton(tr_helper(m_changeDataFolderPushButtonText), this);

    m_chooseDataFolderCheckBox->setVisible(m_changeDataLocationAllowed);
    m_dataFolderLineEdit->setVisible(m_changeDataLocationAllowed);
    m_changeDataFolderPushButton->setVisible(m_changeDataLocationAllowed);

    m_changeInstallationFolderPushButton->setObjectName("changeInstallationFolderPushButton");
    m_changeDataFolderPushButton->setObjectName("changeDataFolderPushButton");

    registerField("folders.installationFolder", m_installationFolderLineEdit);
    registerField("folders.chooseDataFolder", m_chooseDataFolderCheckBox);
    registerField("folders.dataFolder", m_dataFolderLineEdit);

    connect(m_chooseDataFolderCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleFields()));

    connect(m_installationFolderLineEdit, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
    connect(m_chooseDataFolderCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));
    connect(m_dataFolderLineEdit, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));

    m_dataFolderLineEdit->setEnabled(false);
    m_changeDataFolderPushButton->setEnabled(false);

    connect(m_changeInstallationFolderPushButton, SIGNAL(clicked()), this, SLOT(changeInstallationFolderPushButtonClicked()));
    connect(m_changeDataFolderPushButton, SIGNAL(clicked()), this, SLOT(changeDataFolderPushButtonClicked()));

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(m_installationFolderLabel, 0, 0);
    layout->addWidget(m_installationFolderLineEdit, 1, 0);
    layout->addWidget(m_changeInstallationFolderPushButton, 1, 1);
    layout->addWidget(m_chooseDataFolderCheckBox, 2, 0);
    layout->addWidget(m_dataFolderLineEdit, 3, 0);
    layout->addWidget(m_changeDataFolderPushButton, 3, 1);
    setLayout(layout);
}

bool FoldersPage::isComplete() const
{
    if (m_chooseDataFolderCheckBox->isChecked()) {
        return !m_installationFolderLineEdit->text().isEmpty() && !m_dataFolderLineEdit->text().isEmpty();
    } else {
        return !m_installationFolderLineEdit->text().isEmpty();
    }
}

int FoldersPage::nextId() const
{
    return InstallWizard::Page_Shortcuts;
}

void FoldersPage::updateVisibleFields()
{
    const bool enabled = m_chooseDataFolderCheckBox->isChecked();

    m_dataFolderLineEdit->setEnabled(enabled);
    m_changeDataFolderPushButton->setEnabled(enabled);
}

void FoldersPage::changeInstallationFolderPushButtonClicked()
{
    QString folderPath = fileChooserDialog(m_installationFolderLineEdit->text());

    if (!folderPath.isEmpty()) {
        m_installationFolderLineEdit->setText(folderPath);
    }
};

void FoldersPage::changeDataFolderPushButtonClicked()
{
    QString dataPath = fileChooserDialog(m_dataFolderLineEdit->text());

    if (!dataPath.isEmpty()) {
        m_dataFolderLineEdit->setText(dataPath);
    }
};

QString FoldersPage::fileChooserDialog(QString directory)
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::DontUseNativeDialog, false);
    dialog.setDirectory(directory);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        if (fileNames.size() > 0) {
            return fileNames.at(0);
        }
    }
    return "";
}

void FoldersPage::retranslateUi()
{
    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));

    m_installationFolderLabel->setText(translate_helper(className, m_installationFolderLabelText));
    m_changeInstallationFolderPushButton->setText(translate_helper(className, m_changeInstallationFolderPushButtonText));
    m_chooseDataFolderCheckBox->setText(translate_helper(className, m_chooseDataFolderCheckBoxText));
    m_changeDataFolderPushButton->setText(translate_helper(className, m_changeDataFolderPushButtonText));
}
