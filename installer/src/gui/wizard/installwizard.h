#ifndef INSTALLER_INSTALLWIZARD_H
#define INSTALLER_INSTALLWIZARD_H

#include "gui/abstract_translated_ui.h"

#include <QWizard>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QLabel;
class QLineEdit;
class QRadioButton;
class QSpinBox;
class QPushButton;
class QPlainTextEdit;
QT_END_NAMESPACE

class InstallWizard : public QWizard, public AbstractTranslatedUi
{
    Q_OBJECT

    public:
        enum {
            Page_Welcome,
            Page_License,
            Page_Type,
            Page_Folders,
            Page_Shortcuts,
            Page_Proxy,
            Page_Launch,
            Page_ThirdParty,
        };

        InstallWizard(const QString &_appName, bool _changeDataLocationAllowed, QWidget * parent = nullptr);

        bool isCustomInstallation() const;
        void setCustomInstallation(bool _customInstallation);
        QString getInstallationFolder() const;
        bool isDataFolderChosen() const;
        void setDataFolderChosen(bool _folderChosen);
        QString getDataFolder() const;
        bool isLaunchedAppAtStartUp() const;
        bool isProxyUsed() const;
        QString getProxyHostname() const;
        quint16 getProxyPort() const;
        QString getProxyLogin() const;
        QString getProxyPassword() const;
        bool isStartedAppWhenInstalled() const;

        void setInstallationFolder(const QString &_installationFolder);
        void setDataFolder(const QString &_dataFolder);
        void setLaunchedAppAtStartUp(const bool _launchedAppAtStartUp);
        void setStartedAppWhenInstalled(const bool _startedAppWhenInstalled);

        void setProxyUsed(bool _proxyUsed);
        void setProxyHostname(const QString &_hostname);
        void setProxyPort(quint16 _port);
        void setProxyLogin(const QString &_login);
        void setProxyPassword(const QString _password);

        virtual void retranslateUi();
        void print() const;

        // UI strings
        const QString m_nextButtonText;
        const QString m_backButtonText;
        const QString m_finishButtonText;

    protected:
        void keyPressEvent(QKeyEvent * _event);
};

#endif
