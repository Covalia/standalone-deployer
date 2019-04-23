#include "gui/authenticationdialogui.h"
#include "ui_authenticationdialog.h"

#include "gui/style/stylemanager.h"

#include <QPoint>
#include <QRect>
#ifdef Q_OS_WIN
#include <QPropertyAnimation>
#endif

AuthenticationDialogUI::AuthenticationDialogUI(QWidget * _parent, const QString &_siteDescription) :
    QDialog(_parent),
    m_ui(new Ui::AuthenticationDialog),
    m_parent(_parent)
{
    m_ui->setupUi(this);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    StyleManager::transformStyle(this);

    if (!_siteDescription.isEmpty()) {
        m_ui->siteDescription->setText(_siteDescription);
    }

    connect(m_ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));

    m_parent->setWindowOpacity(0.95);

    center();
}

AuthenticationDialogUI::~AuthenticationDialogUI()
{
    delete m_ui;
}

int AuthenticationDialogUI::exec()
{
#ifdef Q_OS_WIN
        this->setWindowOpacity(0.0);
        QPropertyAnimation * anim = new QPropertyAnimation(this, "windowOpacity");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::OutBack);
        anim->setStartValue(0.0);
        anim->setEndValue(1.0);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
#endif
    return QDialog::exec();
}

void AuthenticationDialogUI::center()
{
    QRect geometry = frameGeometry();
    QPoint center = m_parent->geometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

void AuthenticationDialogUI::onButtonClicked(QAbstractButton * _button)
{
    if (_button != nullptr) {
        const QDialogButtonBox::StandardButton standardButton = m_ui->buttonBox->standardButton(_button);
        m_parent->setWindowOpacity(1);
        switch (standardButton) {
            case QDialogButtonBox::StandardButton::Ok:
                accept();
                break;
            case QDialogButtonBox::StandardButton::Cancel:
                reject();
                break;
            default:
                reject();
        }
    }
}

void AuthenticationDialogUI::setLogin(const QString &_login)
{
    m_ui->userEdit->setText(_login);
}

void AuthenticationDialogUI::setPassword(const QString &_password)
{
    m_ui->passwordEdit->setText(_password);
}

QString AuthenticationDialogUI::getLogin() const
{
    return m_ui->userEdit->text();
}

QString AuthenticationDialogUI::getPassword() const
{
    return m_ui->passwordEdit->text();
}
