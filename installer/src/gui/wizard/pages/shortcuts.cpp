#include "gui/wizard/pages/shortcuts.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"

#include <QtWidgets>

ShortcutsPage::ShortcutsPage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Shortcuts options")),
    m_createOfflineShortcutCheckBoxText(QT_TR_NOOP("Add an offline launch shortcut")),
    m_launchAppAtStartUpCheckBoxText(QT_TR_NOOP("Launch the application at start-up")),
    m_createOfflineShortcutCheckBox(nullptr),
    m_launchAppAtStartUpCheckBox(nullptr)
{
    setTitle(tr_helper(m_titleText));

    m_createOfflineShortcutCheckBox = new QCheckBox(tr_helper(m_createOfflineShortcutCheckBoxText), this);
    m_launchAppAtStartUpCheckBox = new QCheckBox(tr_helper(m_launchAppAtStartUpCheckBoxText), this);

    registerField("shortcuts.createOfflineShortcut", m_createOfflineShortcutCheckBox);
    registerField("shortcuts.launchAppAtStartUp", m_launchAppAtStartUpCheckBox);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_createOfflineShortcutCheckBox);
    layout->addWidget(m_launchAppAtStartUpCheckBox);
    setLayout(layout);
}

int ShortcutsPage::nextId() const
{
    return InstallWizard::Page_Proxy;
}

void ShortcutsPage::retranslateUi()
{
    L_INFO("ShortcutsPage::retranslateUi() called");

    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));
    m_createOfflineShortcutCheckBox->setText(translate_helper(className, m_createOfflineShortcutCheckBoxText));
    m_launchAppAtStartUpCheckBox->setText(translate_helper(className, m_launchAppAtStartUpCheckBoxText));
}
