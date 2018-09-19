#ifndef INSTALLER__STYLEMANAGER_H
#define INSTALLER__STYLEMANAGER_H

#include <QString>
#include <QSharedPointer>

#include "settings/resourcesettings.h"

class QWidget;

class StyleManager
{
    public:
        static QString transformStyle(QSharedPointer<ResourceSettings> _resourceSettings, QString _styleSheet);
        static void transformStyle(QSharedPointer<ResourceSettings> _resourceSettings, QWidget * widget);
        static void setGeneralStyle(QSharedPointer<ResourceSettings> _resourceSettings);

    private:
        StyleManager();
        virtual ~StyleManager();
};

#endif
