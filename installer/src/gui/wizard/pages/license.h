#ifndef INSTALLER_WIZARD_PAGE_LICENSE_H
#define INSTALLER_WIZARD_PAGE_LICENSE_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QTextBrowser;
QT_END_NAMESPACE

class LicensePage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT
    public:
        LicensePage(QWidget * parent = nullptr);
        int nextId() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_licenseLabelText;
        const QString m_linkToSourcesLabelText;
        const QString m_agreeLicenseCheckBoxText;

    private:
        QLabel * m_licenseLabel;
        QLabel * m_linkToSourcesLabel;
        QTextBrowser * m_licenseTextBrowser;
        QCheckBox * m_agreeLicenseCheckBox;
};

#endif
