#include "gui/forms/installationui.h"
#include "ui_installation.h"

#include "gui/style/stylemanager.h"
#include "log/logger.h"

InstallationUI::InstallationUI(QSharedPointer<ResourcesSettings> _resourcesSettings, QWidget * _parent) :
    QWidget(_parent),
    m_ui(new Ui::InstallationUI)
{
    L_INFO("Lauche installation widget animation");

    m_ui->setupUi(this);

    StyleManager::transformStyle(_resourcesSettings, this);

    movie = new QMovie();
    movie->setFileName(":/images/loading.gif");

    m_ui->labelGif->setMovie(movie);

    L_INFO("Start gif animation");
    movie->start();
}

InstallationUI::~InstallationUI()
{
    delete m_ui;
    delete movie;
}
