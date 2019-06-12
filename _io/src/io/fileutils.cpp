#include "fileutils.h"
#include <QDir>
#include "log/logger.h"

bool FileUtils::copyDirRecursively(const QString &_fromDir, const QString &_toDir)
{
    if (_fromDir.isEmpty() || _toDir.isEmpty()) {
		L_INFO("either source or destination directory name is empty.");
        return false;
    }

    QDir dir;
    dir.setPath(_fromDir);

    L_INFO(QString("Making directory: %1").arg(_toDir));
    if (!dir.mkpath(_toDir)) {
		L_ERROR("Unable to make directory.");
        return false;
    }

    {
        QStringListIterator iterator(dir.entryList(QDir::Files | QDir::Hidden));
        while(iterator.hasNext()) {
            const QString copy_file = iterator.next();
            const QString from = _fromDir + QDir::separator() + copy_file;
            const QString to = _toDir + QDir::separator() + copy_file;

            if (QFile::exists(to)) {
                L_INFO(QString("Removing file: %1").arg(to));
                if (!QFile::remove(to)) {
					L_ERROR("Unable to remove file.");
                    return false;
                }
            }

            L_INFO(QString("Copying file: %1 to: %2").arg(from).arg(to));
            if (!QFile::copy(from, to)) {
				L_ERROR("Unable to copy file.");
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

bool FileUtils::createDirIfNotExists(const QDir&_dir)
{
    if (_dir.exists()) {
        return true;
    } else {
        bool created = QDir().mkpath(_dir.absolutePath());
        if (created) {
            L_INFO(QString("Success while creating parent directory: %1").arg(_dir.absolutePath()));
        } else {
            L_ERROR(QString("Error while creating parent directory: %1").arg(_dir.absolutePath()));
        }
        return created;
    }
}
