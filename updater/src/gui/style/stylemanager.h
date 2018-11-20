#ifndef INSTALLER__STYLEMANAGER_H
#define INSTALLER__STYLEMANAGER_H

#include <QString>

class QWidget;

class StyleManager
{
    public:
        static QString transformStyle(QString styleSheet);
        static void transformStyle(QWidget * widget);
        static void setGeneralStyle();

    private:
        StyleManager();
        virtual ~StyleManager();
};

#endif
