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
 * It's possible to add widget in content of this window.
 */
class WindowUI : public QMainWindow, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        explicit WindowUI(QWidget * _centralWidget, const QString &_appName, QWidget * _parent = nullptr);
        virtual ~WindowUI();
        virtual void retranslateUi();
        void setLocale(const QString &_locale);
        QString getLocale() const;

        virtual void setCentralWidget(QWidget * _widget);

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
        virtual void closeEvent(QCloseEvent *);
        static QWidget * encloseByWidget(QWidget * _innerWidget);

    signals:
        void changeLanguageSignal(const QString &_language);

    private slots:
        void comboBoxLanguageEvent(int _index);
};

#endif
