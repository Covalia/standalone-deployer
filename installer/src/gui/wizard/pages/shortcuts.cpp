#include "gui/wizard/pages/shortcuts.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"

#include <QtWidgets>

ShortcutsPage::ShortcutsPage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Shortcuts options")),
    m_launchAppAtStartUpCheckBoxText(QT_TR_NOOP("Launch the application at start-up")),
    m_launchAppAtStartUpCheckBox(nullptr)
{
    setTitle(tr_helper(m_titleText));

    m_launchAppAtStartUpCheckBox = new QCheckBox(tr_helper(m_launchAppAtStartUpCheckBoxText), this);

    registerField("shortcuts.launchAppAtStartUp", m_launchAppAtStartUpCheckBox);

    QVBoxLayout * layout = new QVBoxLayout(this);
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
    m_launchAppAtStartUpCheckBox->setText(translate_helper(className, m_launchAppAtStartUpCheckBoxText));
}
