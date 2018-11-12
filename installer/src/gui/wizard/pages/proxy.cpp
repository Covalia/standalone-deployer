#include "gui/wizard/pages/proxy.h"
#include "gui/wizard/installwizard.h"

#include <QtWidgets>

ProxyPage::ProxyPage(QWidget * parent) : QWizardPage(parent),
    m_titleText(QT_TR_NOOP("Proxy configuration")),
    m_useProxyCheckBoxText(QT_TR_NOOP("Use Proxy")),
    m_hostnameLabelText(QT_TR_NOOP("&Hostname")),
    m_portLabelText(QT_TR_NOOP("&Port")),
    m_loginLabelText(QT_TR_NOOP("&Login")),
    m_passwordLabelText(QT_TR_NOOP("&Password")),
    m_useProxyCheckBox(0),
    m_hostnameLabel(0),
    m_hostnameLineEdit(0),
    m_portLabel(0),
    m_portLineEdit(0),
    m_loginLabel(0),
    m_loginLineEdit(0),
    m_passwordLabel(0),
    m_passwordLineEdit(0)
{
    setTitle(tr_helper(m_titleText));

    m_useProxyCheckBox = new QCheckBox(tr_helper(m_useProxyCheckBoxText), this);

    m_hostnameLabel = new QLabel(tr_helper(m_hostnameLabelText), this);
    m_portLabel = new QLabel(tr_helper(m_portLabelText), this);
    m_loginLabel = new QLabel(tr_helper(m_loginLabelText), this);
    m_passwordLabel = new QLabel(tr_helper(m_passwordLabelText), this);

    m_hostnameLineEdit = new QLineEdit(this);
    m_portLineEdit = new QLineEdit(this);
    m_portLineEdit->setValidator(new QIntValidator(1, 65535));
    m_portLineEdit->setText("3128");
    m_loginLineEdit = new QLineEdit(this);
    m_passwordLineEdit = new QLineEdit(this);
    m_passwordLineEdit->setEchoMode(QLineEdit::Password);

    m_hostnameLabel->setBuddy(m_hostnameLineEdit);
    m_portLabel->setBuddy(m_portLineEdit);
    m_loginLabel->setBuddy(m_loginLineEdit);
    m_passwordLabel->setBuddy(m_passwordLineEdit);

    registerField("proxy.use", m_useProxyCheckBox);
    registerField("proxy.hostname", m_hostnameLineEdit);
    registerField("proxy.port", m_portLineEdit);
    registerField("proxy.login", m_loginLineEdit);
    registerField("proxy.password", m_passwordLineEdit);

    connect(m_useProxyCheckBox, SIGNAL(stateChanged(int)), this, SLOT(updateVisibleFields()));

    connect(m_useProxyCheckBox, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));
    connect(m_hostnameLineEdit, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
    connect(m_portLineEdit, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));

    m_hostnameLabel->setEnabled(false);
    m_hostnameLineEdit->setEnabled(false);
    m_portLabel->setEnabled(false);
    m_portLineEdit->setEnabled(false);
    m_loginLabel->setEnabled(false);
    m_loginLineEdit->setEnabled(false);
    m_passwordLabel->setEnabled(false);
    m_passwordLineEdit->setEnabled(false);

    QGridLayout * layout = new QGridLayout(this);
    layout->addWidget(m_useProxyCheckBox, 0, 1);
    layout->addWidget(m_hostnameLabel, 1, 0);
    layout->addWidget(m_hostnameLineEdit, 1, 1);
    layout->addWidget(m_portLabel, 2, 0);
    layout->addWidget(m_portLineEdit, 2, 1);
    layout->addWidget(m_loginLabel, 3, 0);
    layout->addWidget(m_loginLineEdit, 3, 1);
    layout->addWidget(m_passwordLabel, 4, 0);
    layout->addWidget(m_passwordLineEdit, 4, 1);
    setLayout(layout);
}

int ProxyPage::nextId() const
{
    return InstallWizard::Page_Launch;
}

bool ProxyPage::isComplete() const
{
    if (m_useProxyCheckBox->isChecked()) {
        // port is always good, validated by QIntValidator, but must not be empty.
        // authentication is optional.
        return !m_hostnameLineEdit->text().isEmpty() && !m_portLineEdit->text().isEmpty();
    } else {
        return true;
    }
}

void ProxyPage::updateVisibleFields()
{
    const bool enabled = m_useProxyCheckBox->isChecked();

    m_hostnameLabel->setEnabled(enabled);
    m_hostnameLineEdit->setEnabled(enabled);
    m_portLabel->setEnabled(enabled);
    m_portLineEdit->setEnabled(enabled);
    m_loginLabel->setEnabled(enabled);
    m_loginLineEdit->setEnabled(enabled);
    m_passwordLabel->setEnabled(enabled);
    m_passwordLineEdit->setEnabled(enabled);
}

void ProxyPage::retranslateUi()
{
    const QString className = metaObject()->className();

    setTitle(translate_helper(className, m_titleText));

    m_useProxyCheckBox->setText(translate_helper(className, m_useProxyCheckBoxText));
    m_hostnameLabel->setText(translate_helper(className, m_hostnameLabelText));
    m_portLabel->setText(translate_helper(className, m_portLabelText));
    m_loginLabel->setText(translate_helper(className, m_loginLabelText));
    m_passwordLabel->setText(translate_helper(className, m_passwordLabelText));
}
