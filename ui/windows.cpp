#include "windows.h"
#include "ui_windows.h"
#include "../src/style/stylemanager.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLayoutItem>

Windows::Windows(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::Windows){
    ui->setupUi(this);

    this->setAttribute(Qt::WA_QuitOnClose);
    this->setWindowFlags(Qt::FramelessWindowHint);


    StyleManager::transformStyle(this);

    connect(ui->buttonClose, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
}

Windows::~Windows(){
    delete ui;
}

void Windows::center(){
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

void Windows::changeContentWidget(QWidget * widget){
    // delete all old widget
    clearLayout(ui->contentLayout, true);

    ui->contentLayout->addWidget(widget);
}

void Windows::clearLayout(QLayout * layout, bool deleteWidgets){
    while (QLayoutItem * item = layout->takeAt(0)) {
        if (deleteWidgets) {
            if (QWidget * widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout * childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

void Windows::closeEvent(QCloseEvent * _event){
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        // : This string refers to the exit message title.
        int ret = QMessageBox::question(this, tr("Attention !"),
                                        // : This string refers to the exit message.
                                        tr("Vous êtes sur le point de quitter l'application, voulez vous continuer ?"),
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            m_alreadyClosedOnMacOs = true;
            _event->accept();
        } else {
            _event->ignore();
        }
    }
}
