#ifndef UPDATER_CONFIGWINDOW_H
#define UPDATER_CONFIGWINDOW_H

#include <QMainWindow>

class QCloseEvent;
class QMouseEvent;
class ConfigWizard;
class Settings;

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit ConfigWindow(Settings * _settings, QWidget * _parent = nullptr);
        virtual ~ConfigWindow();
        void center();

    protected:
        virtual void mousePressEvent(QMouseEvent *);
        virtual void mouseMoveEvent(QMouseEvent *);
        virtual void closeEvent(QCloseEvent *);

    private slots:
        void wizardFinished(int _result);

    private:
        QPoint m_clickedPosition;
        Ui::ConfigWindow * m_ui;
        Settings * m_settings;
        ConfigWizard * m_configWizard;
        bool m_alreadyClosedOnMacOs = false;
};

#endif
