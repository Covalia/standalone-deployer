#include "about.h"
#include "ui_about.h"
#include "style/stylemanager.h"

#include <QTextStream>
#include <QScrollBar>
#include <QTextCursor>

About::About(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::About){
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    connect(ui->buttonValidateAbout, SIGNAL(clicked()), this, SLOT(validateAbout()));

    QFile f(":/resources/contract.txt");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        ui->plainTextEdit->appendPlainText(in.readAll());
    }
    ui->plainTextEdit->moveCursor(QTextCursor::Start);
    ui->plainTextEdit->ensureCursorVisible();
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
