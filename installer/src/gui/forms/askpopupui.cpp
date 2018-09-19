#include "gui/forms/askpopupui.h"
#include "ui_askpopup.h"

#include "gui/style/stylemanager.h"

#include <QDesktopWidget>
#include <QPoint>
#include <QRect>
#ifdef Q_OS_WIN
#include <QPropertyAnimation>
#endif

AskPopupUI::AskPopupUI(QSharedPointer<ResourceSettings> _resourceSettings, QWidget * _parent, QString _title, QString _description) :
    QDialog(_parent),
    m_ui(new Ui::AskPopupUI),
    m_parent(0)
{
    m_ui->setupUi(this);

    m_parent = _parent;

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    StyleManager::transformStyle(_resourceSettings, this);

    m_ui->labelTitle->setText(_title);
    m_ui->labelDescription->setText(_description);

    connect(m_ui->buttonYes, SIGNAL(clicked()), this, SLOT(yesEvent()));
    connect(m_ui->buttonNo, SIGNAL(clicked()), this, SLOT(noEvent()));

    m_parent->setWindowOpacity(0.95);

    center();
}

AskPopupUI::~AskPopupUI()
{
    delete m_ui;
}

int AskPopupUI::exec()
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

void AskPopupUI::center()
{
    QRect geometry = frameGeometry();
    QPoint center = m_parent->geometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

void AskPopupUI::yesEvent()
{
    m_parent->setWindowOpacity(1);
    accept();
}

void AskPopupUI::noEvent()
{
    m_parent->setWindowOpacity(1);
    reject();
}
