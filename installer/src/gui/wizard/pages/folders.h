#ifndef INSTALLER__WIZARD_PAGE_FOLDERS_H
#define INSTALLER__WIZARD_PAGE_FOLDERS_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class FoldersPage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        FoldersPage(bool _changeDataLocationAllowed, QWidget * parent = 0);
        int nextId() const override;
        bool isComplete() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_installationFolderLabelText;
        const QString m_changeInstallationFolderPushButtonText;
        const QString m_chooseDataFolderCheckBoxText;
        const QString m_changeDataFolderPushButtonText;

    private:
        QLabel * m_installationFolderLabel;
        QLineEdit * m_installationFolderLineEdit;
        QPushButton * m_changeInstallationFolderPushButton;

        QCheckBox * m_chooseDataFolderCheckBox;
        QLineEdit * m_dataFolderLineEdit;
        QPushButton * m_changeDataFolderPushButton;

        bool m_changeDataLocationAllowed;

        QString fileChooserDialog(QString directory);

    private slots:
        void updateVisibleFields();
        void changeInstallationFolderPushButtonClicked();
        void changeDataFolderPushButtonClicked();
};

#endif
