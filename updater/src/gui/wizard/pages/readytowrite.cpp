#include "gui/wizard/pages/readytowrite.h"
#include "gui/wizard/configwizard.h"
#include "log/logger.h"

#include <QtWidgets>

ReadyToWritePage::ReadyToWritePage(QWidget * parent) : QWizardPage(parent),
    m_descriptionLabel(nullptr)
{
    setTitle(tr("Configuration ready to be saved"));

    m_descriptionLabel = new QLabel(tr("Configuration will be written when you will finish this wizard."), this);
    m_descriptionLabel->setWordWrap(true);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_descriptionLabel);
    setLayout(layout);
}

int ReadyToWritePage::nextId() const
{
    return -1;
}
