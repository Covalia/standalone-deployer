#ifndef INSTALLER__LICENSEWIZARD_H
#define INSTALLER__LICENSEWIZARD_H

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
            Page_Launch
        };

        InstallWizard(const QString &_appName, QWidget * parent = 0);

        bool isCustomInstallation() const;
        QString getInstallationFolder() const;
        bool isChosenDataFolder() const;
        QString getDataFolder() const;
        bool isCreatedOfflineShortcut() const;
        bool isLaunchedAppAtStartUp() const;
        bool isUsedProxy() const;
        QString getProxyHostname() const;
        quint16 getProxyPort() const;
        QString getProxyLogin() const;
        QString getProxyPassword() const;
        bool isStartedAppWhenInstalled() const;

        void setInstallationFolder(const QString _installationFolder);
        void setDataFolder(const QString _dataFolder);
        void setCreatedOfflineShortcut(const bool _createdOfflineShortcut);
        void setLaunchedAppAtStartUp(const bool _launchedAppAtStartUp);
        void setStartedAppWhenInstalled(const bool _startedAppWhenInstalled);

        virtual void retranslateUi();

        // UI strings
        const QString m_nextButtonText;
        const QString m_backButtonText;
        const QString m_finishButtonText;

    protected:
        void keyPressEvent(QKeyEvent * _event);
};

#endif
