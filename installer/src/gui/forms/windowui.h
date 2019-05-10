#ifndef INSTALLER_WINDOW_H
#define INSTALLER_WINDOW_H

#include <QMainWindow>

class QWidget;
class QLabel;
class QComboBox;

#include "gui/abstract_translated_ui.h"

class QStyledItemDelegate;

/**
 * @class WindowUI
 * @brief Window frame.
 * This window is the enveloppe of application. It contain, close button, change language button.
 * This window is movable.
 * It's possible to add widget in content of this window.
 */
class WindowUI : public QMainWindow, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        explicit WindowUI(QWidget * _centralWidget, const QString &_appName, QWidget * _parent = nullptr);
        virtual ~WindowUI();
        void center();
        virtual void retranslateUi();
        void setLocale(const QString &_locale);
        QString getLocale() const;

        // UI strings
        const QString m_closeActionText;
        const QString m_titleLabelText;

    private:
        const QString m_appName;

        QAction * m_closeAction;

        QToolBar * m_toolbar;
        QLabel * m_iconLabel;
        QLabel * m_titleLabel;
        QComboBox * m_comboBoxLanguage;

        bool m_alreadyClosedOnMacOs = false;
        QPoint m_position;
        void mousePressEvent(QMouseEvent * _e);
        void mouseReleaseEvent(QMouseEvent * _e);
        void mouseMoveEvent(QMouseEvent *);
        virtual void closeEvent(QCloseEvent *);

    signals:
        void changeLanguageSignal(const QString &_language);

    private slots:
        void comboBoxLanguageEvent(int _index);
};

#endif
