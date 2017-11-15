#include "welcome.h"
#include "ui_welcome.h"
#include "../src/style/stylemanager.h"

Welcome::Welcome(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::Welcome){
    ui->setupUi(this);

    StyleManager::transformStyle(this);
}

Welcome::~Welcome(){
    delete ui;
}
