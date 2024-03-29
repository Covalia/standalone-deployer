#include "gui/wizard/pages/license.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"
#include "io/config.h"

#include <QtWidgets>

LicensePage::LicensePage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("License agreement")),
    m_licenseLabelText(QT_TR_NOOP("This software installer is distributed under the terms of the GNU General Public License version 3 (GPLv3).")),
    m_linkToSourcesLabelText(QT_TR_NOOP("Source code can be downloaded from: %1")),
    m_agreeLicenseCheckBoxText(QT_TR_NOOP("I agree to the terms of the license")),
    m_licenseLabel(nullptr),
    m_linkToSourcesLabel(nullptr),
    m_licenseTextBrowser(nullptr),
    m_agreeLicenseCheckBox(nullptr)
{
    setTitle(tr_helper(m_titleText));

    m_licenseLabel = new QLabel(tr_helper(m_licenseLabelText), this);
    m_licenseLabel->setWordWrap(true);
    m_agreeLicenseCheckBox = new QCheckBox(tr_helper(m_agreeLicenseCheckBoxText), this);

    m_licenseTextBrowser = new QTextBrowser(this);
    m_licenseTextBrowser->setReadOnly(true);
    m_licenseTextBrowser->setOpenExternalLinks(true);

    const QString urlLink = QString("<a href='%1'>%1</a>").arg(IOConfig::SourceCodeUrl);
    m_linkToSourcesLabel = new QLabel(tr_helper(m_linkToSourcesLabelText).arg(urlLink), this);
    m_linkToSourcesLabel->setWordWrap(true);
    m_linkToSourcesLabel->setTextFormat(Qt::RichText);
    m_linkToSourcesLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    m_linkToSourcesLabel->setOpenExternalLinks(true);

    QFile f(":/license/gpl-3.0.txt");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        m_licenseTextBrowser->insertHtml(in.readAll());
        f.close();
    }

    m_licenseTextBrowser->moveCursor(QTextCursor::Start);
    m_licenseTextBrowser->ensureCursorVisible();

    registerField("license.agree*", m_agreeLicenseCheckBox);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_licenseLabel);
    layout->addWidget(m_linkToSourcesLabel);
    layout->addWidget(m_licenseTextBrowser);
    layout->addWidget(m_agreeLicenseCheckBox);
    setLayout(layout);
}

int LicensePage::nextId() const
{
    return InstallWizard::Page_ThirdParty;
}

void LicensePage::retranslateUi()
{
    L_INFO("LicensePage::retranslateUi() called");

    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));
    m_licenseLabel->setText(translate_helper(className, m_licenseLabelText));
    m_agreeLicenseCheckBox->setText(translate_helper(className, m_agreeLicenseCheckBoxText));
    const QString urlLink = QString("<a href='%1'>%1</a>").arg(IOConfig::SourceCodeUrl);
    m_linkToSourcesLabel->setText(translate_helper(className, m_linkToSourcesLabelText).arg(urlLink));
}
