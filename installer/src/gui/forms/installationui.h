#ifndef INSTALLATIONUI_H
#define INSTALLATIONUI_H

#include <QWidget>
#include <QMovie>
#include <QSharedPointer>

#include "settings/resourcessettings.h"

namespace Ui {
class InstallationUI;
}

class InstallationUI : public QWidget
{
    Q_OBJECT

    public:
        explicit InstallationUI(QSharedPointer<ResourcesSettings> _resourcesSettings, QWidget * _parent = 0);
        virtual ~InstallationUI();

    private:
        Ui::InstallationUI * m_ui;
        QMovie * movie;
};

#endif // INSTALLATIONUI_H
