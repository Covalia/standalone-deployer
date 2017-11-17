#include "personnalize.h"
#include "ui_personnalize.h"
#include "../src/style/stylemanager.h"

#include "windows.h"

Personnalize::Personnalize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Personnalize)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);

//    connect(Windows, SIGNAL(Windows::changeLanguageSignal()), this, SLOT(changeLanguage()));
}

Personnalize::~Personnalize()
{
    delete ui;
}


void Personnalize::changeLanguage()
{
   ui->retranslateUi(this);
}
