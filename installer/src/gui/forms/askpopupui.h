#ifndef INSTALLER_ASKPOPUPUI_H
#define INSTALLER_ASKPOPUPUI_H

#include <QDialog>
#include <QString>

namespace Ui {
class AskPopupUI;
}

/**
 * @class AskPopupUI
 * @brief AskPopup dialog window
 */
class AskPopupUI : public QDialog
{
    Q_OBJECT

    public:
        explicit AskPopupUI(QWidget * _parent = nullptr, QString _title = "", QString _description = "");
        virtual ~AskPopupUI();
        int exec();

    private:
        Ui::AskPopupUI * m_ui;
        QWidget * m_parent;
        void center();

    private slots:
        void yesEvent();
        void noEvent();
};

#endif
