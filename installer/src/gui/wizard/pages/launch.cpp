#include "gui/wizard/pages/launch.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"

#include <QtWidgets>

LaunchPage::LaunchPage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Launching")),
    m_startAppWhenInstalledCheckBoxText(QT_TR_NOOP("Start the application when closing")),
    m_startAppWhenInstalledCheckBox(0)
{
    setTitle(tr_helper(m_titleText));

    m_startAppWhenInstalledCheckBox = new QCheckBox(tr_helper(m_startAppWhenInstalledCheckBoxText), this);
    registerField("launch.startAppWhenInstalled", m_startAppWhenInstalledCheckBox);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_startAppWhenInstalledCheckBox);
    setLayout(layout);
}

int LaunchPage::nextId() const
{
    return -1;
}

void LaunchPage::retranslateUi()
{
	L_INFO("LaunchPage::retranslateUi() called");

    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));
    m_startAppWhenInstalledCheckBox->setText(translate_helper(className, m_startAppWhenInstalledCheckBoxText));
}
