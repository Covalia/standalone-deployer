#include "gui/wizard/pages/license.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"

#include <QtWidgets>

LicensePage::LicensePage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("License agreement")),
    m_licenseLabelText(QT_TR_NOOP("This software installer is distributed under the terms of the GNU General Public License version 3 (GPLv3).")),
    m_agreeLicenseCheckBoxText(QT_TR_NOOP("I agree to the terms of the license")),
    m_licenseLabel(0),
    m_licensePlainTextEdit(0),
    m_agreeLicenseCheckBox(0)
{
    setTitle(tr_helper(m_titleText));

    m_licenseLabel = new QLabel(tr_helper(m_licenseLabelText), this);
    m_licenseLabel->setWordWrap(true);
    m_agreeLicenseCheckBox = new QCheckBox(tr_helper(m_agreeLicenseCheckBoxText), this);

    m_licensePlainTextEdit = new QPlainTextEdit(this);
    m_licensePlainTextEdit->setReadOnly(true);

    QFile f(":/resources/contract.txt");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        m_licensePlainTextEdit->appendPlainText(in.readAll());
        f.close();
    }

    m_licensePlainTextEdit->moveCursor(QTextCursor::Start);
    m_licensePlainTextEdit->ensureCursorVisible();

    registerField("license.agree*", m_agreeLicenseCheckBox);

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_licenseLabel);
    layout->addWidget(m_licensePlainTextEdit);
    layout->addWidget(m_agreeLicenseCheckBox);
    setLayout(layout);
}

int LicensePage::nextId() const
{
    return InstallWizard::Page_Type;
}

void LicensePage::retranslateUi()
{
	L_INFO("LicensePage::retranslateUi() called");

    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));
    m_licenseLabel->setText(translate_helper(className, m_licenseLabelText));
    m_agreeLicenseCheckBox->setText(translate_helper(className, m_agreeLicenseCheckBoxText));
}
