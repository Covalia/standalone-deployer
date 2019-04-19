#ifndef INSTALLER_WIZARD_PAGE_TYPE_H
#define INSTALLER_WIZARD_PAGE_TYPE_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLabel;
class QRadioButton;
QT_END_NAMESPACE

class TypePage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT
    public:
        TypePage(QWidget * parent = nullptr);
        int nextId() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_topLabelText;
        const QString m_simpleInstallationRadioButtonText;
        const QString m_customInstallationRadioButtonText;

    private:
        QLabel * m_topLabel;
        QRadioButton * m_simpleInstallationRadioButton;
        QRadioButton * m_customInstallationRadioButton;
};

#endif
