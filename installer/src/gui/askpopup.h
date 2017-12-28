#ifndef INSTALLER__ASKPOPUP_H
#define INSTALLER__ASKPOPUP_H

#include <QDialog>
#include <QString>

namespace Ui {
class AskPopup;
}

/**
 * @class AskPopup
 * @brief AskPopup dialog window
 */
class AskPopup : public QDialog
{
    Q_OBJECT

    public:
        explicit AskPopup(QWidget * _parent = 0, QString _title = "", QString _description = "");
        virtual ~AskPopup();
        int exec();

    private:
        Ui::AskPopup * m_ui;
        QWidget * m_parent;
        void center();

    private slots:
        void yesEvent();
        void noEvent();
};

#endif // ASKPOPUP_H
