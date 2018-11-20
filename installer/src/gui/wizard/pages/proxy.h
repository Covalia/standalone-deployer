#ifndef INSTALLER__WIZARD_PAGE_PROXY_H
#define INSTALLER__WIZARD_PAGE_PROXY_H

#include "gui/abstract_translated_ui.h"

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class ProxyPage : public QWizardPage, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        ProxyPage(QWidget * parent = 0);
        int nextId() const override;
        bool isComplete() const override;
        // bool validatePage() override; // TODO check proxy connection
        void retranslateUi() override;

        // UI strings
        const QString m_titleText;
        const QString m_useProxyCheckBoxText;
        const QString m_hostnameLabelText;
        const QString m_portLabelText;
        const QString m_loginLabelText;
        const QString m_passwordLabelText;

    private:
        QCheckBox * m_useProxyCheckBox;
        QLabel * m_hostnameLabel;
        QLineEdit * m_hostnameLineEdit;
        QLabel * m_portLabel;
        QLineEdit * m_portLineEdit;
        QLabel * m_loginLabel;
        QLineEdit * m_loginLineEdit;
        QLabel * m_passwordLabel;
        QLineEdit * m_passwordLineEdit;

    private slots:
        void updateVisibleFields();
};

#endif
