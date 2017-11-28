#ifndef ASKPOPUP_H
#define ASKPOPUP_H

#include <QDialog>
#include <QString>

namespace Ui {
class AskPopup;
}

/**
 * @class AskPopup
 * @brief AskPopup dialog windows
 */
class AskPopup : public QDialog
{
    Q_OBJECT

public:
    explicit AskPopup(QWidget * m_parent = 0, QString title = "", QString description = "");
    ~AskPopup();
    int exec();

private:
    Ui::AskPopup * ui;
    QWidget * m_parent;
    void center();

private slots:
    void yesEvent();
    void noEvent();
};

#endif // ASKPOPUP_H
