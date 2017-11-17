#include "welcome.h"
#include "ui_welcome.h"
#include "../src/style/stylemanager.h"
#include <QDebug>

Welcome::Welcome(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Welcome){
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    connect(ui->buttonCustomInstall, SIGNAL(clicked()), this, SLOT(customInstallationEvent()));
    //connect label to slot
}

Welcome::~Welcome(){
    delete ui;
}

void Welcome::changeLanguage() {
    ui->retranslateUi(this);
}

void Welcome::customInstallationEvent() {
    customInstallationSignal();
}

//dans slot -> signal
