#ifndef WINDOWS_H
#define WINDOWS_H

#include <QMainWindow>

namespace Ui {
class Windows;
}

class Windows : public QMainWindow
{
    Q_OBJECT

public:
    explicit Windows(QWidget *parent = 0);
    ~Windows();

    void center();

    void changeContentWidget(QWidget * widget);

private:
    Ui::Windows *ui;

    bool m_alreadyClosedOnMacOs = false;

    void clearLayout(QLayout* layout, bool deleteWidgets);
    virtual void closeEvent(QCloseEvent *);
};

#endif // WINDOWS_H
