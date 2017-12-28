#include "gui/window.h"
#include "ui_window.h"

#include "style/stylemanager.h"
#include "lang/languagemanager.h"
#include "log/logger.h"
#include "gui/personalize.h"
#include "gui/askpopup.h"
#include "settings/resourcessettings.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLayoutItem>
#include <QStyledItemDelegate>

Window::Window(QWidget * _parent) :
    QMainWindow(_parent),
    m_ui(new Ui::Window),
    m_itemDelegate(0)
{
    m_ui->setupUi(this);

    setWindowTitle(tr("Stand-alone deployment"));

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    StyleManager::transformStyle(this);

    connect(m_ui->buttonClose, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
    connect(m_ui->buttonAbout, SIGNAL(clicked()), this, SLOT(aboutEvent()));

    //windows title
    ResourcesSettings * resource = ResourcesSettings::getInstance();
    m_ui->labelTitle->setText(tr("Installation of %1").arg(resource->getApp_name()));

    // combobox init
    m_itemDelegate = new QStyledItemDelegate();
    m_ui->comboBoxLanguage->setItemDelegate(m_itemDelegate);
    m_ui->comboBoxLanguage->addItem("Francais", QVariant("fr_FR"));
    m_ui->comboBoxLanguage->addItem("English", QVariant("en_US"));
    connect(m_ui->comboBoxLanguage, SIGNAL(activated(int)), this, SLOT(comboBoxLanguageEvent(int)));

    // bug combobox style
    StyleManager::transformStyle(this);
}

Window::~Window()
{
    delete m_ui;
    delete m_itemDelegate;
}

void Window::center()
{
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    m_position = center;
    move(geometry.topLeft());
}

void Window::mousePressEvent(QMouseEvent * _e)
{
    m_position = QPoint(_e->globalX() - geometry().x(), _e->globalY() - geometry().y());
    setCursor(QCursor(Qt::SizeAllCursor));
}

void Window::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(QCursor(Qt::ArrowCursor));
}

void Window::mouseMoveEvent(QMouseEvent * _e)
{
    QWidget::mouseMoveEvent(_e);
    if (_e->buttons() && Qt::LeftButton) {
        QPoint toMove = _e->globalPos() - m_position;
        move(toMove);
        repaint();
    }
}

void Window::changeContentWidget(QWidget * _widget)
{
    // hide all old widgets
    hideLayoutContent(m_ui->contentLayout);
    m_ui->contentLayout->addWidget(_widget);
    _widget->setVisible(true);
}

void Window::setVisibleButton(bool _about, bool _changeLanguage)
{
    m_ui->buttonAbout->setVisible(_about);
    m_ui->comboBoxLanguage->setVisible(_changeLanguage);
}

void Window::hideLayoutContent(QLayout * _layout)
{
    QLayoutItem * i = _layout->takeAt(0);

    if (i) {
        QWidget * w = i->widget();
        if (w) {
            w->setVisible(false);
        }
    }
}

void Window::closeEvent(QCloseEvent * _event)
{
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopup * popupClose = new AskPopup(this, tr("Do you want to exit the application?"), tr("The installation will be stopped"));
        popupClose->show();
        if (popupClose->exec() == QDialog::Accepted) {
            _event->accept();
            m_alreadyClosedOnMacOs = true;
        } else {
            _event->ignore();
        }
    }
}

void Window::aboutEvent()
{
    emit aboutSignal();
}

void Window::comboBoxLanguageEvent(int _index)
{
    L_INFO("Detect language change in language combobox");
    QString dateValue = m_ui->comboBoxLanguage->itemData(_index).toString();
    if (!dateValue.isNull() && !dateValue.isEmpty()) {
        LanguageManager::updateLanguage(dateValue);
        emit changeLanguageSignal();
    }
}

void Window::changeLanguage()
{
    m_ui->retranslateUi(this);
}
