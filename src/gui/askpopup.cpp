#include "gui/askpopup.h"
#include "ui_askpopup.h"
#include "style/stylemanager.h"

#include <QPoint>
#include <QRect>
#include <QDesktopWidget>
#include <QPropertyAnimation>

AskPopup::AskPopup(QWidget * parent, QString title, QString description) :
    QDialog(parent),
    ui(new Ui::AskPopup)
{
    ui->setupUi(this);

    m_parent = parent;

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    StyleManager::transformStyle(this);

    ui->labelTitle->setText(title);
    ui->labelDescription->setText(description);

    connect(ui->buttonYes, SIGNAL(clicked()), this, SLOT(yesEvent()));
    connect(ui->buttonNo, SIGNAL(clicked()), this, SLOT(noEvent()));

    m_parent->setWindowOpacity(0.95);

    center();
}

AskPopup::~AskPopup()
{
    delete ui;
    delete m_parent;
}

int AskPopup::exec()
{
    this->setWindowOpacity(0.0);
    QPropertyAnimation * anim = new QPropertyAnimation(this, "windowOpacity");
    anim->setDuration(500);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
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
