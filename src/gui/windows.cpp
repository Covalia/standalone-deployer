#include "windows.h"
#include "ui_windows.h"

#include "../src/style/stylemanager.h"
#include "../src/language/languagemanager.h"
#include "../src/log/simpleqtlogger.h"
#include "personnalize.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include <QLayoutItem>
#include <QStyledItemDelegate>

Windows::Windows(QWidget * parent) :
    QMainWindow(parent),
    ui(new Ui::Windows){
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

void Windows::changeContentWidget(QWidget * widget, bool deleteWidgets){
    // delete all old widget
    clearLayout(ui->contentLayout, deleteWidgets);
    ui->contentLayout->addWidget(widget);
}


void Windows::clearLayout(QLayout * layout, bool deleteWidgets){
    while (QLayoutItem * item = layout->takeAt(0)) {
        if (QWidget * widget = item->widget()){
            //layout->removeWidget(widget);
            if (deleteWidgets) {
                widget->deleteLater();
            }
            else{
                widget->setParent(this);
            }
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

void Windows::aboutEvent()
{
   aboutSignal();
}

void Windows::comboBoxLanguageEvent(int index){
    L_INFO("Detect language change in language combobox");
    QString dateValue = ui->comboBoxLanguage->itemData(index).toString();
    if(!dateValue.isNull() && !dateValue.isEmpty()){
         LanguageManager::updateLanguage(dateValue);
         changeLanguageSignal();
    }
}


void Windows::changeLanguage()
{
   ui->retranslateUi(this);
}




