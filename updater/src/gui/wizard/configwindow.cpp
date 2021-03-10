#include "gui/wizard/configwindow.h"

#include <QDebug>
#include <QCloseEvent>
#include <QDesktopWidget>
#include <QMouseEvent>

#include "gui/askpopupui.h"
#include "gui/wizard/configwizard.h"
#include "ui_configwindow.h"
#include "settings/settings.h"
#include "utils.h"

ConfigWindow::ConfigWindow(Settings * _settings, QWidget * _parent) :
    QMainWindow(_parent),
    m_ui(new Ui::ConfigWindow),
    m_settings(_settings),
    m_configWizard(nullptr)
{
    setAttribute(Qt::WA_QuitOnClose);
    setWindowFlags(Qt::FramelessWindowHint);

    m_ui->setupUi(this);

    AppPath appPath = Utils::getAppPath();
    m_ui->closeButton->setIcon(QIcon(":/images/close.png"));
    m_ui->titleIconLabel->setPixmap(QPixmap(appPath.getImagesDir().absoluteFilePath("title.png")));

    connect(m_ui->closeButton, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));

    // _settings must not be null...
    QString appName = "";
    bool proxyUsed = false;
    QString proxyHostname = "";
    quint16 proxyPort = 1;
    QString proxyLogin = "";
    QString proxyPassword = "";

    if (_settings) {
        appName = _settings->getAppName();
        proxyUsed = _settings->isProxyUsed();
        proxyHostname = _settings->getProxyHostname();
        proxyPort = static_cast<quint16>(_settings->getProxyPort());
        proxyLogin = _settings->getProxyLogin();
        proxyPassword = _settings->getProxyPassword();
    }

    m_configWizard = new ConfigWizard(appName, this);
    m_configWizard->setProxyUsed(proxyUsed);
    m_configWizard->setProxyHostname(proxyHostname);
    m_configWizard->setProxyPort(proxyPort);
    m_configWizard->setProxyLogin(proxyLogin);
    m_configWizard->setProxyPassword(proxyPassword);

    connect(m_configWizard, SIGNAL(finished(int)), this, SLOT(wizardFinished(int)));

    m_ui->wizardVerticalLayout->removeWidget(m_ui->wizardWidget);
    m_ui->wizardVerticalLayout->addWidget(m_configWizard);
}

ConfigWindow::~ConfigWindow()
{
    delete m_ui;
    delete m_configWizard;
    // do not delete m_settings, it's a singleton!
}

void ConfigWindow::closeEvent(QCloseEvent * _event)
{
    qDebug() << "closeEvent";
    // sous macos, lors de la fermeture via command+q, on passe deux fois dans cet event.
    if (m_alreadyClosedOnMacOs) {
        _event->accept();
    } else {
        AskPopupUI * popupClose = new AskPopupUI(this, "", tr("The configuration will not be saved."));
        popupClose->show();
        if (popupClose->exec() == QDialog::Accepted) {
            _event->accept();
            m_alreadyClosedOnMacOs = true;
        } else {
            _event->ignore();
        }
    }
}

void ConfigWindow::mousePressEvent(QMouseEvent * _event)
{
    m_clickedPosition = _event->pos();
}

void ConfigWindow::mouseMoveEvent(QMouseEvent * _event)
{
    move(_event->globalPos() - m_clickedPosition);
}

void ConfigWindow::center()
{
    QRect geometry = frameGeometry();
    QPoint center = QDesktopWidget().availableGeometry().center();

    geometry.moveCenter(center);
    move(geometry.topLeft());
}

void ConfigWindow::wizardFinished(int _result)
{
    Q_UNUSED(_result)

    // m_settings must not be null...
    if (m_settings) {
        // write settings
        m_settings->setProxyUsed(m_configWizard->isProxyUsed());
        m_settings->setProxyHostname(m_configWizard->getProxyHostname());
        m_settings->setProxyPort(m_configWizard->getProxyPort());
        m_settings->setProxyLogin(m_configWizard->getProxyLogin());
        m_settings->setProxyPassword(m_configWizard->getProxyPassword());
        m_settings->writeSettings();
    }
    qApp->quit();
}
