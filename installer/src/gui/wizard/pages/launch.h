#ifndef INSTALLER_WIZARD_PAGE_LAUNCH_H
#define INSTALLER_WIZARD_PAGE_LAUNCH_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
QT_END_NAMESPACE

class LaunchPage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT
    public:
        LaunchPage(QWidget * parent = 0);
        int nextId() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_startAppWhenInstalledCheckBoxText;

    private:
        QCheckBox * m_startAppWhenInstalledCheckBox;
};

#endif
