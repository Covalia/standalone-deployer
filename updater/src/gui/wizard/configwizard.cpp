#include <QtWidgets>
#include <QtDebug>
#include "gui/wizard/configwizard.h"
#include "gui/wizard/pages/welcome.h"
#include "gui/wizard/pages/proxy.h"
#include "gui/wizard/pages/readytowrite.h"
#include "log/logger.h"

ConfigWizard::ConfigWizard(const QString &_appName, QWidget * parent) : QWizard(parent)
{
    setPage(Page_Welcome, new WelcomePage(_appName));
    setPage(Page_Proxy, new ProxyPage);
    setPage(Page_ReadyToWrite, new ReadyToWritePage);

    setStartId(Page_Welcome);
    // want the Back button to be non-destructive and keep the values entered by the user
    setOption(QWizard::IndependentPages, true);
    setOption(QWizard::NoBackButtonOnStartPage, true);
    setOption(QWizard::NoCancelButton, true);

    setWizardStyle(ModernStyle);

    setButtonText(QWizard::NextButton, tr("Next >"));
    setButtonText(QWizard::BackButton, tr("< Back"));
    setButtonText(QWizard::FinishButton, tr("Finish"));
}

bool ConfigWizard::isProxyUsed() const
{
    return field("proxy.use").toBool();
}

QString ConfigWizard::getProxyHostname() const
{
    return field("proxy.hostname").toString();
}

quint16 ConfigWizard::getProxyPort() const
{
    return static_cast<quint16>(field("proxy.port").toUInt());
}

QString ConfigWizard::getProxyLogin() const
{
    return field("proxy.login").toString();
}

QString ConfigWizard::getProxyPassword() const
{
    return field("proxy.password").toString();
}

void ConfigWizard::keyPressEvent(QKeyEvent * _event)
{
    // we disable wizard closing by escape key.
    if (_event->key() != Qt::Key_Escape) {
        QWizard::keyPressEvent(_event);
    }
}

void ConfigWizard::setProxyUsed(bool _proxyUsed)
{
    setField("proxy.use", _proxyUsed);
}

void ConfigWizard::setProxyHostname(const QString &_hostname)
{
    setField("proxy.hostname", _hostname);
}

void ConfigWizard::setProxyPort(quint16 _port)
{
    setField("proxy.port", _port);
}

void ConfigWizard::setProxyLogin(const QString &_login)
{
    setField("proxy.login", _login);
}

void ConfigWizard::setProxyPassword(const QString _password)
{
    setField("proxy.password", _password);
}

void ConfigWizard::print() const
{
    qDebug() << "----------------------------------------";
    qDebug() << "isProxyUsed(): " << QString(isProxyUsed() ? "yes" : "no");
    qDebug() << "getProxyHostname(): " << getProxyHostname();
    qDebug() << "getProxyPort(): " << QString::number(getProxyPort());
    qDebug() << "getProxyLogin(): " << getProxyLogin();
    qDebug() << "getProxyPassword(): " << getProxyPassword();
    qDebug() << "----------------------------------------";
}
