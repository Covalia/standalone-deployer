#ifndef UPDATER_CONFIGWIZARD_H
#define UPDATER_CONFIGWIZARD_H

#include <QWizard>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QLineEdit;
QT_END_NAMESPACE

class ConfigWizard : public QWizard
{
    Q_OBJECT

    public:

        enum {
            Page_Welcome,
            Page_Proxy,
            Page_ReadyToWrite,
        };

        ConfigWizard(const QString &_appName, QWidget * parent = nullptr);

        bool isProxyUsed() const;
        QString getProxyHostname() const;
        quint16 getProxyPort() const;
        QString getProxyLogin() const;
        QString getProxyPassword() const;

        void setProxyUsed(bool _proxyUsed);
        void setProxyHostname(const QString &_hostname);
        void setProxyPort(quint16 _port);
        void setProxyLogin(const QString &_login);
        void setProxyPassword(const QString _password);

        void print() const;

    protected:
        void keyPressEvent(QKeyEvent * _event);
};

#endif
