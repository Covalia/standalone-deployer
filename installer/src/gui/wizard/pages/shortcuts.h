#ifndef INSTALLER_WIZARD_PAGE_SHORTCUTS_H
#define INSTALLER_WIZARD_PAGE_SHORTCUTS_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
QT_END_NAMESPACE

class ShortcutsPage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        ShortcutsPage(QWidget * parent = nullptr);
        int nextId() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_launchAppAtStartUpCheckBoxText;

    private:
        QCheckBox * m_launchAppAtStartUpCheckBox;
};

#endif
