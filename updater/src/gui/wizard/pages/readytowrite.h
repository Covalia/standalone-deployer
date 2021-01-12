#ifndef UPDATER_WIZARD_PAGE_READYTOWRITE_H
#define UPDATER_WIZARD_PAGE_READYTOWRITE_H

#include <QWizardPage>

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class ReadyToWritePage : public QWizardPage
{
    Q_OBJECT

    public:
        ReadyToWritePage(QWidget * parent = nullptr);
        int nextId() const override;

    private:
        QLabel * m_descriptionLabel;
};

#endif
