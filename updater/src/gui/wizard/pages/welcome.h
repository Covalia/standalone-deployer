#ifndef UPDATER_WIZARD_PAGE_WELCOME_H
#define UPDATER_WIZARD_PAGE_WELCOME_H

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class WelcomePage : public QWizardPage
{
    Q_OBJECT

    public:
        WelcomePage(const QString &_appName, QWidget * parent = nullptr);
        int nextId() const override;

    private:
        const QString m_appName;
        QLabel * m_topLabel;
        QLabel * m_bottomLabel;
};

#endif
