#include "gui/about.h"
#include "ui_about.h"
#include "style/stylemanager.h"

#include <QTextStream>
#include <QScrollBar>
#include <QTextCursor>

About::About(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::About)
{
    m_ui->setupUi(this);

    // link style
    m_ui->buttonValidateAbout->setAccessibleName("pageButton");

    StyleManager::transformStyle(this);

    connect(m_ui->buttonValidateAbout, SIGNAL(clicked()), this, SLOT(validateAbout()));

    initTextContract();
}

About::~About()
{
    delete m_ui;
}

void About::initTextContract()
{
    QFile f(":/resources/contract.txt");

    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&f);
        m_ui->plainTextEdit->appendPlainText(in.readAll());
    }

    m_ui->plainTextEdit->moveCursor(QTextCursor::Start);
    m_ui->plainTextEdit->ensureCursorVisible();

    f.close();
}

void About::changeLanguage()
{
    m_ui->retranslateUi(this);
    initTextContract();
}

void About::validateAbout()
{
    emit validateAboutSignal();
}
