#include "manager/resources/macosresources.h"

#include <QDir>
#include <QCoreApplication>

#include "io/fileutils.h"
#include "log/simpleqtlogger.h"

MacosResources::MacosResources(const AppPath _appPath) :
    m_appPath(_appPath)
{
}

MacosResources::~MacosResources()
{
}

bool MacosResources::extractResources()
{
    bool extractResources = true;

    QDir dir(QCoreApplication::applicationDirPath());

    dir.cdUp();
    dir.cd("Resources");
    const QString imagesDirPath = dir.absoluteFilePath("images");

    if (FileUtils::copyDirRecursively(imagesDirPath, m_appPath.getImagesDir().absolutePath())) {
        L_INFO("images directory copied from app resources.");
    } else {
        L_ERROR("Unable to copy images directory from app resources.");
        extractResources = false;
    }

    const QString styleCssFileName = "style.css";
    const QString styleCssFileDest = m_appPath.getConfigurationDir().absoluteFilePath(styleCssFileName);
    if (QFile::exists(styleCssFileDest)) {
        L_INFO(QString("%1 already exists.").arg(styleCssFileDest));
        if (QFile::remove(styleCssFileDest)) {
            L_INFO(QString("%1 removed.").arg(styleCssFileDest));
        } else {
            L_ERROR(QString("Unable to remove %1").arg(styleCssFileDest));
        }
    }

    if (QFile::copy(dir.absoluteFilePath(styleCssFileName), styleCssFileDest)) {
        L_INFO("style.css file copied from app resources.");
    } else {
        L_ERROR("Unable to copy style.css file from app resources.");
        extractResources = false;
    }

    return extractResources;
}

bool MacosResources::extractProjectIniToTempFile(const QString &_toPath)
{
    return writeFileToTempFile("project.ini", _toPath);
}

bool MacosResources::extractStyleCssToTempFile(const QString &_toPath)
{
    return writeFileToTempFile("style.css", _toPath);
}

bool MacosResources::extractTitlePngToTempFile(const QString &_toPath)
{
    return writeFileToTempFile("images/title.png", _toPath);
}

bool MacosResources::writeFileToTempFile(const QString &_resourceFile, const QString &_toPath)
{
    QDir dir(QCoreApplication::applicationDirPath());

    dir.cdUp();
    dir.cd("Resources");
    const QString resourceFilePath = dir.absoluteFilePath(_resourceFile);

    const QFileInfo fileInfo(_toPath);
    const QDir parentDir = fileInfo.dir();
    const bool created = FileUtils::createDirIfNotExists(parentDir);

    if (created) {
        bool result = true;

        QFile sourceFile(resourceFilePath);

        if (sourceFile.exists()) {
            if (sourceFile.open(QIODevice::ReadOnly)) {
                L_INFO(QString("Resource source file to copy opened in readonly: %1").arg(resourceFilePath));
                QByteArray bytes = sourceFile.readAll();

                QFile destFile(_toPath);

                if (destFile.open(QIODevice::WriteOnly)) {
                    L_INFO(QString("Resource destination file opened in writeonly: %1").arg(_toPath));

                    if (destFile.write(bytes) == -1) {
                        L_ERROR(QString("Unable to write resource destination file: %1").arg(_toPath));
                        result = false;
                    } else {
                        L_INFO(QString("Resource destination file written: %1").arg(_toPath));
                        result = true;
                    }

                    destFile.close();
                } else {
                    L_ERROR(QString("Unable to open resource destination file for write: %1").arg(_toPath));
                    result = false;
                }

                sourceFile.close();
            } else {
                L_ERROR(QString("Unable to open resource source file for read: %1").arg(resourceFilePath));
                result = false;
            }
        } else {
            L_ERROR(QString("Resource source file does not exist: %1").arg(resourceFilePath));
            result = false;
        }

        return result;
    } else {
        L_ERROR(QString("Unable to create directory %1").arg(parentDir.absolutePath()));
        return false;
    }
}
