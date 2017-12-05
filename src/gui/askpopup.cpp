#include "gui/askpopup.h"
#include "ui_askpopup.h"
#include "style/stylemanager.h"

#include <QPoint>
#include <QRect>
#include <QDesktopWidget>
#if Q_OS_WIN
#include <QPropertyAnimation>
#endif

AskPopup::AskPopup(QWidget * _parent, QString _title, QString _description) :
    QDialog(_parent),
    m_ui(new Ui::AskPopup),
    m_parent(0)
{
    m_ui->setupUi(this);

    m_parent = _parent;

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    StyleManager::transformStyle(this);

    m_ui->labelTitle->setText(_title);
    m_ui->labelDescription->setText(_description);

    connect(m_ui->buttonYes, SIGNAL(clicked()), this, SLOT(yesEvent()));
    connect(m_ui->buttonNo, SIGNAL(clicked()), this, SLOT(noEvent()));

    m_parent->setWindowOpacity(0.95);

    center();
}

AskPopup::~AskPopup()
{
    delete m_ui;
}

int AskPopup::exec()
{
#if Q_OS_WIN
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

void AskPopup::center()
{
    QRect geometry = frameGeometry();
    QPoint center = m_parent->geometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

void AskPopup::yesEvent()
{
    m_parent->setWindowOpacity(1);
    accept();
}

void AskPopup::noEvent()
{
    m_parent->setWindowOpacity(1);
    reject();
}
