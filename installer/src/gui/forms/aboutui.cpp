#include "gui/forms/aboutui.h"
#include "ui_about.h"

#include "gui/style/stylemanager.h"

#include <QScrollBar>
#include <QTextCursor>
#include <QTextStream>

AboutUI::AboutUI(QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::AboutUI)
{
    m_ui->setupUi(this);

    // link style
    m_ui->buttonValidateAbout->setAccessibleName("pageButton");

    StyleManager::transformStyle(this);

    connect(m_ui->buttonValidateAbout, SIGNAL(clicked()), this, SLOT(validateAbout()));

    initTextContract();
}

AboutUI::~AboutUI()
{
    delete m_ui;
}

void AboutUI::initTextContract()
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

void AboutUI::changeLanguage()
{
    m_ui->retranslateUi(this);
    initTextContract();
}

void AboutUI::validateAbout()
{
    emit validateAboutSignal();
}
