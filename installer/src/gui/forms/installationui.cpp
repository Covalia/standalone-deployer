#include "gui/forms/installationui.h"
#include "ui_installation.h"



#include "gui/style/stylemanager.h"


InstallationUI::InstallationUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InstallationUI)
{
    ui->setupUi(this);

    StyleManager::transformStyle(this);

    movie = new QMovie();
    movie->setFileName(":/images/loading.gif");

    ui->labelGif->setMovie(movie);
    movie->start();
}

InstallationUI::~InstallationUI()
{
    delete ui;
    delete movie;
}
