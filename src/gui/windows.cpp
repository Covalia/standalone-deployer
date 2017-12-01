#include "gui/windows.h"
#include "ui_windows.h"

#include "style/stylemanager.h"
#include "language/languagemanager.h"
#include "log/logger.h"
#include "gui/personnalize.h"
#include "gui/askpopup.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLayoutItem>
#include <QStyledItemDelegate>

Windows::Windows(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::Windows)
{
    ui->setupUi(this);

    this->setWindowTitle(tr("Stand-alone deployment"));

    this->setAttribute(Qt::WA_QuitOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);

    StyleManager::transformStyle(this);

    connect(ui->buttonClose, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
    connect(ui->buttonAbout, SIGNAL(clicked()), this, SLOT(aboutEvent()));

    // combobox init
    QStyledItemDelegate * itemDelegate = new QStyledItemDelegate();
    ui->comboBoxLanguage->setItemDelegate(itemDelegate);
    ui->comboBoxLanguage->addItem("Francais", QVariant("fr_FR"));
    ui->comboBoxLanguage->addItem("English", QVariant("en_US"));
    connect(ui->comboBoxLanguage, SIGNAL(activated(int)), this, SLOT(comboBoxLanguageEvent(int)));

    // bug combobox style
    StyleManager::transformStyle(this);
}

Windows::~Windows()
{
    delete ui;
}

void Windows::center()
{
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    m_position = center;
    move(geometry.topLeft());
}

void Windows::mousePressEvent(QMouseEvent * e)
{
    m_position = QPoint(e->globalX() - geometry().x(), e->globalY() - geometry().y());
    this->setCursor(QCursor(Qt::SizeAllCursor));
}

void Windows::mouseReleaseEvent(QMouseEvent *)
{
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void Windows::mouseMoveEvent(QMouseEvent * e)
{
    QWidget::mouseMoveEvent(e);
    if (e->buttons() && Qt::LeftButton) {
        QPoint toMove = e->globalPos() - m_position;
        move(toMove);
        this->repaint();
    }
}

void Windows::changeContentWidget(QWidget * widget, bool deleteWidgets)
{
    // delete all old widget
    clearLayout(ui->contentLayout, deleteWidgets);
    ui->contentLayout->addWidget(widget);
}

void Windows::setVisibleButton(bool about, bool changeLanguage)
{
    ui->buttonAbout->setVisible(about);
    ui->comboBoxLanguage->setVisible(changeLanguage);
}

void Windows::clearLayout(QLayout * layout, bool deleteWidgets)
{
    while (QLayoutItem * item = layout->takeAt(0)) {
        if (QWidget * widget = item->widget()) {
            // layout->removeWidget(widget);
            if (deleteWidgets) {
                widget->deleteLater();
            } else {
                widget->setParent(this);
            }
        }

        if (QLayout * childLayout = item->layout()) {
            clearLayout(childLayout, deleteWidgets);
        }

        delete item;
    }
}

void Windows::closeEvent(QCloseEvent * _event)
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

void Windows::aboutEvent()
{
    aboutSignal();
}

void Windows::comboBoxLanguageEvent(int index)
{
    L_INFO("Detect language change in language combobox");
    QString dateValue = ui->comboBoxLanguage->itemData(index).toString();
    if (!dateValue.isNull() && !dateValue.isEmpty()) {
        LanguageManager::updateLanguage(dateValue);
        changeLanguageSignal();
    }
}

void Windows::changeLanguage()
{
    ui->retranslateUi(this);
}
