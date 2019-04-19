#include "gui/wizard/pages/type.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"

#include <QtWidgets>

TypePage::TypePage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Installation type")),
    m_topLabelText(QT_TR_NOOP("Simple installation uses default settings. Custom installation allows you to choose installation folders, configure the proxy...")),
    m_simpleInstallationRadioButtonText(QT_TR_NOOP("&Simple installation")),
    m_customInstallationRadioButtonText(QT_TR_NOOP("&Custom installation")),
    m_topLabel(nullptr),
    m_simpleInstallationRadioButton(nullptr),
    m_customInstallationRadioButton(nullptr)
{
    setTitle(tr_helper(m_titleText));

    m_topLabel = new QLabel(tr_helper(m_topLabelText), this);
    m_topLabel->setWordWrap(true);

    m_simpleInstallationRadioButton = new QRadioButton(tr_helper(m_simpleInstallationRadioButtonText), this);
    m_customInstallationRadioButton = new QRadioButton(tr_helper(m_customInstallationRadioButtonText), this);
    m_simpleInstallationRadioButton->setChecked(true);

    registerField("type.simple", m_simpleInstallationRadioButton);
    registerField("type.custom", m_customInstallationRadioButton);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_topLabel);
    layout->addWidget(m_simpleInstallationRadioButton);
    layout->addWidget(m_customInstallationRadioButton);
    setLayout(layout);
}

int TypePage::nextId() const
{
    if (m_customInstallationRadioButton->isChecked()) {
        return InstallWizard::Page_Folders;
    } else {
        return InstallWizard::Page_Launch;
    }
}

void TypePage::retranslateUi()
{
    L_INFO("TypePage::retranslateUi() called");

    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));
    m_topLabel->setText(translate_helper(className, m_topLabelText));
    m_simpleInstallationRadioButton->setText(translate_helper(className, m_simpleInstallationRadioButtonText));
    m_customInstallationRadioButton->setText(translate_helper(className, m_customInstallationRadioButtonText));
}
