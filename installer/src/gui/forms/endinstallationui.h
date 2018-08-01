#ifndef ENDINSTALLATIONUI_H
#define ENDINSTALLATIONUI_H

#include <QWidget>

namespace Ui {
class EndInstallationUI;
}

class EndInstallationUI : public QWidget
{
    Q_OBJECT

    public:
        explicit EndInstallationUI(QWidget * parent = 0);
        virtual ~EndInstallationUI();

        void showErrors(QStringList _errors);

    private:
        Ui::EndInstallationUI * m_ui;

    private slots:
        void closeInstallationEvent();

    signals:
        void closeInstallationSignal(bool _launchApplication);
};

#endif // ENDINSTALLATIONUI_H
