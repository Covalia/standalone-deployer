#include "about.h"
#include "ui_about.h"
#include "style/stylemanager.h"

About::About(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::About){
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    connect(ui->buttonValidateAbout, SIGNAL(clicked()), this, SLOT(validateAbout()));
}

About::~About(){
    delete ui;
}

void About::changeLanguage(){
    ui->retranslateUi(this);
}

void About::validateAbout(){
    validateAboutSignal();
}
