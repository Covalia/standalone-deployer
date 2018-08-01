#ifndef INSTALLATIONUI_H
#define INSTALLATIONUI_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class InstallationUI;
}

class InstallationUI : public QWidget
{
    Q_OBJECT

    public:
        explicit InstallationUI(QWidget * parent = 0);
        virtual ~InstallationUI();

    private:
        Ui::InstallationUI * m_ui;
        QMovie * movie;
};

#endif // INSTALLATIONUI_H
