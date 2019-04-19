#ifndef INSTALLER_INSTALLATIONUI_H
#define INSTALLER_INSTALLATIONUI_H

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
        explicit InstallationUI(QWidget * _parent = nullptr);
        virtual ~InstallationUI();

        virtual void retranslateUi();

    private:
        Ui::InstallationUI * m_ui;
        QMovie * movie;
};

#endif
