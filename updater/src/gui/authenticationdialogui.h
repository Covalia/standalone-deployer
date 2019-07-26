#ifndef UPDATER_AUTHENTICATIONDIALOGUI_H
#define UPDATER_AUTHENTICATIONDIALOGUI_H

#include <QDialog>
#include <QString>

namespace Ui {
class AuthenticationDialogUI;
}

class QAbstractButton;

/**
 * @class AuthenticationDialogUI
 * @brief AuthenticationDialogUI dialog window
 */
class AuthenticationDialogUI : public QDialog
{
    Q_OBJECT

    public:
        explicit AuthenticationDialogUI(QWidget * _parent = nullptr, const QString &_siteDescription = "");
        virtual ~AuthenticationDialogUI();
        int exec();

        void setLogin(const QString &_login);
        void setPassword(const QString &_password);
        QString getLogin() const;
        QString getPassword() const;

    private:
        Ui::AuthenticationDialogUI * m_ui;
        QWidget * m_parent;
        void center();

    private slots:
        void onButtonClicked(QAbstractButton * _button);
};

#endif
