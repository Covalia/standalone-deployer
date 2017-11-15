#include "personnalize.h"
#include "ui_personnalize.h"
#include "../src/style/stylemanager.h"

Personnalize::Personnalize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Personnalize)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);
}

Personnalize::~Personnalize()
{
    delete ui;
}
