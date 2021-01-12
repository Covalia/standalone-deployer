#ifndef UPDATER_WIZARD_PAGE_PROXY_H
#define UPDATER_WIZARD_PAGE_PROXY_H

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class ProxyPage : public QWizardPage
{
    Q_OBJECT

    public:
        ProxyPage(QWidget * parent = nullptr);
        bool isComplete() const override;
        int nextId() const override;
    // bool validatePage() override; // TODO check proxy connection

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
