#ifndef INSTALLER__WINDOW_H
#define INSTALLER__WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QSharedDataPointer>

#include "settings/resourcesettings.h"

namespace Ui {
class WindowUI;
}

class QStyledItemDelegate;

/**
 * @class WindowUI
 * @brief Window frame.
 * This window is the enveloppe of application. It contain, close button, change language button, about button.
 * This window is movable.
 * It's possible to add widget in content of this window.
 */
class WindowUI : public QMainWindow
{
    Q_OBJECT

    public:
        explicit WindowUI(QSharedPointer<ResourceSettings> _resourceSettings, QWidget * _parent = 0);
        virtual ~WindowUI();
        void center();
        void changeContentWidget(QWidget * _widget);
        void setVisibleButton(bool _about, bool _changeLanguage);
        QWidget * currentWidgetCentral() const;

    private:
        QSharedPointer<ResourceSettings> m_resourceSettings;

        Ui::WindowUI * m_ui;
        QStyledItemDelegate * m_itemDelegate;
        bool m_alreadyClosedOnMacOs = false;
        QPoint m_position;
        void mousePressEvent(QMouseEvent * _e);
        void mouseReleaseEvent(QMouseEvent * _e);
        void mouseMoveEvent(QMouseEvent *);
        void hideLayoutContent(QLayout * _layout);
        virtual void closeEvent(QCloseEvent *);
        void updateUi();

    signals:
        void changeLanguageSignal();
        void aboutSignal();

    private slots:
        void comboBoxLanguageEvent(int _index);
        void changeLanguage();
        void aboutEvent();
};

#endif // INSTALLER__WINDOW_H
