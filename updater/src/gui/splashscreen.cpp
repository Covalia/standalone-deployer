
#include <QMessageBox>
#include <QCloseEvent>
#include <QDesktopWidget>

#include "gui/askpopupui.h"
#include "ui_splashscreen.h"
#include "gui/splashscreen.h"
#include "gui/style/stylemanager.h"
#include "log/logger.h"

/*!
 *
 * \brief Constructor
 *
 */
Splashscreen::Splashscreen(QWidget * _parent) :
    QMainWindow(_parent),
    m_ui(new Ui::Splashscreen)
{
    m_ui->setupUi(this);

    StyleManager::transformStyle(this);

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    movie = new QMovie();
    movie->setFileName(":/images/loading.gif");

    m_ui->labelGif->setMovie(movie);

    L_INFO("Start gif animation");
    movie->start();
}

/*!
 * \brief Main application windows destructor
 */
Splashscreen::~Splashscreen()
{
    delete m_ui;
    delete movie;
}



void Splashscreen::closeEvent(QCloseEvent * _event)
{
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopupUI * popupClose = new AskPopupUI(this, tr("Do you want to exit the application?"), tr("The installation will be stopped"));
        popupClose->show();
        if (popupClose->exec() == QDialog::Accepted) {
            _event->accept();
            m_alreadyClosedOnMacOs = true;
        } else {
            _event->ignore();
        }
    }
}

/*!
 * \brief MainWindow::mousePressEvent
 * \param _event
 */
void Splashscreen::mousePressEvent(QMouseEvent * _event)
{
    m_clickedPosition = _event->pos();
}

/*!
 * \brief MainWindow::mouseMoveEvent
 * \param _event
 */
void Splashscreen::mouseMoveEvent(QMouseEvent * _event)
{
    move(_event->globalPos() - m_clickedPosition);
}

void Splashscreen::center()
{
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

