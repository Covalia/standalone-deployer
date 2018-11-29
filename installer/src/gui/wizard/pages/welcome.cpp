#include "gui/wizard/pages/welcome.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"

#include <QtWidgets>

WelcomePage::WelcomePage(const QString &_appName, QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Welcome to the %1 setup wizard")),
    m_topLabelText(QT_TR_NOOP("This wizard will guide you through the installation of %1.")),
    m_bottomLabelText(QT_TR_NOOP("Click Next to continue.")),
    m_appName(_appName),
    m_topLabel(0),
    m_bottomLabel(0)
{
    setTitle(tr_helper(m_titleText).arg(m_appName));

    m_topLabel = new QLabel(tr_helper(m_topLabelText).arg(m_appName), this);
    m_topLabel->setWordWrap(true);

    m_bottomLabel = new QLabel(tr_helper(m_bottomLabelText), this);
    m_bottomLabel->setWordWrap(true);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_topLabel);
    layout->addWidget(m_bottomLabel);
    setLayout(layout);
}

int WelcomePage::nextId() const
{
    return InstallWizard::Page_License;
}

void WelcomePage::retranslateUi()
{
	L_INFO("WelcomePage::retranslateUi() called");

    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText).arg(m_appName));
    m_topLabel->setText(translate_helper(className, m_topLabelText).arg(m_appName));
    m_bottomLabel->setText(translate_helper(className, m_bottomLabelText));
}
