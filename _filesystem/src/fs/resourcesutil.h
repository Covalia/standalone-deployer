#ifndef RESOURCESUTIL_H
#define RESOURCESUTIL_H

#include <QString>
#include <QPair>

class ResourcesUtil
{
    public:
        ResourcesUtil();

        static QString getLoaderResourcesPath();
        static QString getUpdaterResourcesPath();

        static QString getLoaderPath(QString installLocation);
        static QString getUpdaterPath(QString installLocation);

        static QPair<bool, QString> extractResourceToPath(QString resourcePath, QString copyFilePath);

    private:
        static QString getExtension();
};

#endif // RESOURCESUTIL_H
