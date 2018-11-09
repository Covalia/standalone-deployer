#ifndef INSTALLER__WIZARD_PAGE_SHORTCUTS_H
#define INSTALLER__WIZARD_PAGE_SHORTCUTS_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
QT_END_NAMESPACE

class ShortcutsPage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        ShortcutsPage(QWidget * parent = 0);
        int nextId() const override;
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_createOfflineShortcutCheckBoxText;
        const QString m_launchAppAtStartUpCheckBoxText;

    private:
        QCheckBox * m_createOfflineShortcutCheckBox;
        QCheckBox * m_launchAppAtStartUpCheckBox;
};

#endif
