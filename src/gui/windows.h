#ifndef WINDOWS_H
#define WINDOWS_H

#include <QMainWindow>
#include <QWidget>

namespace Ui {
class Windows;
}

/**
 * @class Windows
 * @brief Windows frame.
 * This windows is the enveloppe of application. It contain, close button, change language button, about button.
 * This windows is movable.
 * It's possible to add widget in content of this windows.
 */
class Windows : public QMainWindow
{
    Q_OBJECT

    public:
        explicit Windows(QWidget * parent = 0);
        ~Windows();
        void center();
        void changeContentWidget(QWidget * widget, bool deleteWidgets = true);
        void setVisibleButton(bool about, bool changeLanguage);
        QWidget * currentWidgetCentral() const;

    private:
        Ui::Windows * ui;
        bool m_alreadyClosedOnMacOs = false;
        QPoint m_position;
        void mousePressEvent(QMouseEvent * e);
        void mouseReleaseEvent(QMouseEvent * e);
        void mouseMoveEvent(QMouseEvent *);
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
