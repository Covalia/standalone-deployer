#ifndef INSTALLER__WIZARD_PAGE_WELCOME_H
#define INSTALLER__WIZARD_PAGE_WELCOME_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class WelcomePage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        WelcomePage(const QString &_appName, QWidget * parent = 0);

        int nextId() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_topLabelText;
        const QString m_bottomLabelText;

        const QString m_appName;

    private:
        QLabel * m_topLabel;
        QLabel * m_bottomLabel;
};

#endif
