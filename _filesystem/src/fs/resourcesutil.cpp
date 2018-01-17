#include "resourcesutil.h"

#include <QFile>

#include "fs/config.h"

ResourcesUtil::ResourcesUtil()
{
}

QString ResourcesUtil::getLoaderResourcesPath()
{
    return ":/bin/" + FileSystemConfig::LoaderFile + getExtension();
}

QString ResourcesUtil::getUpdaterResourcesPath()
{
    return ":/bin/" + FileSystemConfig::UpdaterFile + getExtension();
}

QString ResourcesUtil::getLoaderPath(QString installLocation)
{
    return installLocation + "/" + FileSystemConfig::LoaderFile + getExtension();
}

QString ResourcesUtil::getUpdaterPath(QString installLocation)
{
    return installLocation + "/" +  FileSystemConfig::UpdaterDir + "/" + FileSystemConfig::UpdaterFile + getExtension();
}

QString ResourcesUtil::getExtension()
{
    QString extension = "";

    #ifdef _WIN32
        extension = ".exe";
    #elif TARGET_OS_MAC
        extension = ".dmg";
    #endif

    return extension;
}

QPair<bool, QString> ResourcesUtil::extractResourceToPath(QString resourcePath, QString copyFilePath)
{
    if (QFile::exists(copyFilePath)) {
        QFile f(copyFilePath);
        f.setPermissions(QFile::WriteOther);
        bool remove = f.remove();
        if (!remove) {
            QPair<bool, QString> error = qMakePair(false, "Error when remove file" + copyFilePath);
            return error;
        }
    }
    bool success = QFile::copy(resourcePath, copyFilePath);
    if (success) {
        QPair<bool, QString> error = qMakePair(true, "Success extracting " + copyFilePath);
        return error;
    } else {
        QPair<bool, QString> error = qMakePair(false, "An error occurred when extracting " + copyFilePath);
        return error;
    }
}
