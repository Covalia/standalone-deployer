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
        ~EndInstallationUI();

        void showError(QString error);

    private:
        Ui::EndInstallationUI * ui;

    private slots:
        void closeInstallationEvent();

    signals:
        void closeInstallationSignal(bool launchApplication);
};

#endif // ENDINSTALLATIONUI_H