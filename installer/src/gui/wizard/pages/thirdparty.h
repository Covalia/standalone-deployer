#ifndef INSTALLER_WIZARD_PAGE_THIRDPARTY_H
#define INSTALLER_WIZARD_PAGE_THIRDPARTY_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLabel;
class QTextBrowser;
QT_END_NAMESPACE

class ThirdPartyPage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT
    public:
        ThirdPartyPage(QWidget * parent = 0);
        int nextId() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_thirdPartyLabelText;

    private:
        QLabel * m_thirdPartyLabel;
        QTextBrowser * m_thirdPartyTextBrowser;
};

#endif
