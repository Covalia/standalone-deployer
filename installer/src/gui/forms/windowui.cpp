#include "gui/forms/windowui.h"
#include "ui_window.h"

#include "gui/forms/askpopupui.h"
#include "gui/forms/personalizeui.h"
#include "gui/style/stylemanager.h"
#include "lang/languagemanager.h"
#include "log/logger.h"
#include "settings/resourcessettings.h"

#include <QCloseEvent>
#include <QDesktopWidget>
#include <QLayoutItem>
#include <QMessageBox>
#include <QStyledItemDelegate>

WindowUI::WindowUI(QWidget * _parent) :
    QMainWindow(_parent),
    m_ui(new Ui::WindowUI),
    m_itemDelegate(0)
{
    m_ui->setupUi(this);

    setWindowTitle(tr("Standalone deployment"));

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    StyleManager::transformStyle(this);

    connect(m_ui->buttonClose, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
    connect(m_ui->buttonAbout, SIGNAL(clicked()), this, SLOT(aboutEvent()));

    // combobox init
    m_itemDelegate = new QStyledItemDelegate();
    m_ui->comboBoxLanguage->setItemDelegate(m_itemDelegate);
    m_ui->comboBoxLanguage->addItem("Francais", QVariant("fr_FR"));
    m_ui->comboBoxLanguage->addItem("English", QVariant("en_US"));
    connect(m_ui->comboBoxLanguage, SIGNAL(activated(int)), this, SLOT(comboBoxLanguageEvent(int)));

    //windows title
    updateUi();

    // bug combobox style
    StyleManager::transformStyle(this);
}

WindowUI::~WindowUI()
{
    delete m_ui;
    delete m_itemDelegate;
}


void WindowUI::center()
{
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    m_position = center;
    move(geometry.topLeft());
}


void WindowUI::mousePressEvent(QMouseEvent * _e)
{
    m_position = QPoint(_e->globalX() - geometry().x(), _e->globalY() - geometry().y());
    setCursor(QCursor(Qt::SizeAllCursor));
}

void WindowUI::mouseReleaseEvent(QMouseEvent *)
{
    setCursor(QCursor(Qt::ArrowCursor));
}

void WindowUI::mouseMoveEvent(QMouseEvent * _e)
{
    QWidget::mouseMoveEvent(_e);
    if (_e->buttons() && Qt::LeftButton) {
        QPoint toMove = _e->globalPos() - m_position;
        move(toMove);
        repaint();
    }
}

void WindowUI::changeContentWidget(QWidget * _widget)
{
    // hide all old widgets
    hideLayoutContent(m_ui->contentLayout);
    m_ui->contentLayout->addWidget(_widget);
    _widget->setVisible(true);
}

void WindowUI::setVisibleButton(bool _about, bool _changeLanguage)
{
    m_ui->buttonAbout->setVisible(_about);
    m_ui->comboBoxLanguage->setVisible(_changeLanguage);
}

void WindowUI::hideLayoutContent(QLayout * _layout)
{
    QLayoutItem * i = _layout->takeAt(0);

    if (i) {
        QWidget * w = i->widget();
        if (w) {
            w->setVisible(false);
        }
    }
}

void WindowUI::closeEvent(QCloseEvent * _event)
{
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopupUI * popupClose = new AskPopupUI(this, tr("Do you want to exit the application?"), tr("The installation will be stopped"));
        popupClose->show();
        if (popupClose->exec() == QDialog::Accepted) {
            _event->accept();
            m_alreadyClosedOnMacOs = true;
        } else {
            _event->ignore();
        }
    }
}

void WindowUI::aboutEvent()
{
    emit aboutSignal();
}

void WindowUI::comboBoxLanguageEvent(int _index)
{
    L_INFO("Detect language change in language combobox");
    QString dateValue = m_ui->comboBoxLanguage->itemData(_index).toString();
    if (!dateValue.isNull() && !dateValue.isEmpty()) {
        LanguageManager::updateLanguage(dateValue);
        emit changeLanguageSignal();
    }
}

void WindowUI::updateUi(){
    m_ui->retranslateUi(this);
    ResourcesSettings * resource = ResourcesSettings::getInstance();
    m_ui->labelTitle->setText(tr("Installation of %1").arg(resource->getApp_name()));
}

void WindowUI::changeLanguage()
{
    updateUi();
}
