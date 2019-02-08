#include "gui/wizard/pages/thirdparty.h"
#include "gui/wizard/installwizard.h"
#include "log/logger.h"

#include <QtWidgets>

ThirdPartyPage::ThirdPartyPage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Third-party resources and softwares")),
    m_thirdPartyLabelText(QT_TR_NOOP("This software installer may contain third-party resources and softwares which require additional notice.")),
    m_thirdPartyLabel(0),
    m_thirdPartyTextBrowser(0)
{
    setTitle(tr_helper(m_titleText));

    m_thirdPartyLabel = new QLabel(tr_helper(m_thirdPartyLabelText), this);
    m_thirdPartyLabel->setWordWrap(true);

    m_thirdPartyTextBrowser = new QTextBrowser(this);
    m_thirdPartyTextBrowser->setReadOnly(true);
    m_thirdPartyTextBrowser->setOpenExternalLinks(true);

    QFile f(":/resources/third-party.txt");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        m_thirdPartyTextBrowser->insertHtml(in.readAll());
        f.close();
    }

    m_thirdPartyTextBrowser->moveCursor(QTextCursor::Start);
    m_thirdPartyTextBrowser->ensureCursorVisible();

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_thirdPartyLabel);
    layout->addWidget(m_thirdPartyTextBrowser);
    setLayout(layout);
}

int ThirdPartyPage::nextId() const
{
    return InstallWizard::Page_Type;
}

void ThirdPartyPage::retranslateUi()
{
    L_INFO("ThirdPartyPage::retranslateUi() called");

    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));
    m_thirdPartyLabel->setText(translate_helper(className, m_thirdPartyLabelText));
}
