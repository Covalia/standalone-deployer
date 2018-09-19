#ifndef INSTALLER__STYLEMANAGER_H
#define INSTALLER__STYLEMANAGER_H

#include <QString>
#include <QSharedPointer>

#include "settings/resourcessettings.h"

class QWidget;

class StyleManager
{
    public:
        static QString transformStyle(QSharedPointer<ResourcesSettings> _resourcesSettings, QString _styleSheet);
        static void transformStyle(QSharedPointer<ResourcesSettings> _resourcesSettings, QWidget * widget);
        static void setGeneralStyle(QSharedPointer<ResourcesSettings> _resourcesSettings);

    private:
        StyleManager();
        virtual ~StyleManager();
};

#endif
