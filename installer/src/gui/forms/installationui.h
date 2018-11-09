#ifndef INSTALLATIONUI_H
#define INSTALLATIONUI_H

#include "gui/abstract_translated_ui.h"

#include <QWidget>
#include <QMovie>

namespace Ui {
class InstallationUI;
}

class InstallationUI : public QWidget, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        explicit InstallationUI(QWidget * _parent = 0);
        virtual ~InstallationUI();

        virtual void retranslateUi();

    private:
        Ui::InstallationUI * m_ui;
        QMovie * movie;
};

#endif // INSTALLATIONUI_H
