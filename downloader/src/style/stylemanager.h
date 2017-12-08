#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QString>
#include <QWidget>

class StyleManager
{
    public:
        StyleManager();
        static QString transformStyle(QString styleSheet);
        static void transformStyle(QWidget * widget);
        static void setGeneralStyle();
};

#endif // STYLEMANAGER_H
