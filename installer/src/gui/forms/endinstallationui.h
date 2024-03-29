#ifndef INSTALLER_ENDINSTALLATIONUI_H
#define INSTALLER_ENDINSTALLATIONUI_H

#include "gui/abstract_translated_ui.h"

#include <QWidget>

namespace Ui {
class EndInstallationUI;
}

class EndInstallationUI : public QWidget, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        explicit EndInstallationUI(QWidget * _parent = nullptr);
        virtual ~EndInstallationUI();

        void showErrors(QStringList _errors);
        void showSuccess();
        void setStartedAppWhenInstalled(bool _start);

        virtual void retranslateUi();

    private:
        Ui::EndInstallationUI * m_ui;
        bool m_start;
        QStringList m_errors;

    private slots:
        void closeInstallationEvent();

    signals:
        void quitInstaller(bool _startApp);
};

#endif
