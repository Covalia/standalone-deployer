#ifndef UPDATER__SPLASHSCREEN_H
#define UPDATER__SPLASHSCREEN_H

#include <QMainWindow>
#include <QMovie>

class QCloseEvent;
class QMouseEvent;

namespace Ui {
class Splashscreen;
}

class Splashscreen : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Splashscreen(QWidget * _parent = 0);
        ~Splashscreen();
        void center();

    protected:
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

    private:
        Ui::Splashscreen * m_ui;
        bool m_alreadyClosedOnMacOs = false;
        QPoint m_clickedPosition;
        QMovie * movie;
        virtual void closeEvent(QCloseEvent *);
};

#endif
