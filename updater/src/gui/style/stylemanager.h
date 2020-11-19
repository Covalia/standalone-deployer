#ifndef INSTALLER__STYLEMANAGER_H
#define INSTALLER__STYLEMANAGER_H

#include <QString>

class StyleManager
{
    public:
        static void setGeneralStyle(const QString &_styleName);

    private:
        StyleManager();
        virtual ~StyleManager();
};

#endif
