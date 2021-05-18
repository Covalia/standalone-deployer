#include "gui/wizard/pages/welcome.h"
#include "gui/wizard/configwizard.h"
#include "log/logger.h"

#include <QtWidgets>

WelcomePage::WelcomePage(const QString &_appName, QWidget * parent) : QWizardPage(parent),
    m_appName(_appName),
    m_topLabel(nullptr),
    m_bottomLabel(nullptr)
{
    setTitle(tr("Welcome to the %1 config wizard").arg(m_appName));

    m_topLabel = new QLabel(tr("This wizard will help you to configure %1.").arg(m_appName), this);
    m_topLabel->setWordWrap(true);

    m_bottomLabel = new QLabel(tr("Click Next to continue."), this);
    m_bottomLabel->setWordWrap(true);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_topLabel);
    layout->addWidget(m_bottomLabel);
    setLayout(layout);
}

int WelcomePage::nextId() const
{
    return ConfigWizard::Page_Proxy;
}
