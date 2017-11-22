#ifndef WINDOWS_H
#define WINDOWS_H

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class Windows;
}

class Windows : public QMainWindow
{
    Q_OBJECT

public:
    explicit Windows(QWidget * parent = 0);
    ~Windows();

    void center();

    void changeContentWidget(QWidget * widget, bool deleteWidgets = true);

    QWidget *currentWidgetCentral() const;

private:
    Ui::Windows * ui;

    bool m_alreadyClosedOnMacOs = false;


    void clearLayout(QLayout * layout, bool deleteWidgets = true);
    virtual void closeEvent(QCloseEvent *);

signals:
    void changeLanguageSignal();
    void aboutSignal();

private slots:
    void comboBoxLanguageEvent(int index);
    void changeLanguage();
    void aboutEvent();
};

#endif // WINDOWS_H
