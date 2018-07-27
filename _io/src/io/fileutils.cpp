#include "fileutils.h"
#include <QDir>
#include "log/logger.h"

bool FileUtils::copyDirRecursively(const QString &_fromDir, const QString &_toDir)
{
    if (_fromDir.isEmpty() || _toDir.isEmpty()) {
        return false;
    }

    QDir dir;
    dir.setPath(_fromDir);

    L_INFO("make directory: " + _toDir);
    if (!dir.mkpath(_toDir)) {
        return false;
    }

    {
        QStringListIterator iterator(dir.entryList(QDir::Files | QDir::Hidden));
        while(iterator.hasNext()) {
            const QString copy_file = iterator.next();
            const QString from = _fromDir + QDir::separator() + copy_file;
            const QString to = _toDir + QDir::separator() + copy_file;

            if (QFile::exists(to)) {
                L_INFO("remove file: " + to);
                if (!QFile::remove(to)) {
                    return false;
                }
            }

            L_INFO("copy file: " + from + " to: " + to);
            if (!QFile::copy(from, to)) {
                return false;
            }
        }
    }

    {
        QStringListIterator iterator(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden));
        while (iterator.hasNext()) {
            const QString copy_dir = iterator.next();
            const QString from = _fromDir + QDir::separator() + copy_dir;
            const QString to = _toDir + QDir::separator() + copy_dir;

            if (!copyDirRecursively(from, to)) {
                return false;
            }
        }
    }

    return true;
}

bool FileUtils::removeDirRecursively(const QString &_dir)
{
    if (_dir.isEmpty()) {
        return false;
    }
    QDir dir(_dir);
    return dir.removeRecursively();
}

bool FileUtils::directoryExists(const QString &_dir)
{
    if (_dir.isEmpty()) {
        return false;
    }
    QDir dir(_dir);
    return dir.exists();
}
