#ifndef FILESYSTEM__FILEUTILS_H
#define FILESYSTEM__FILEUTILS_H

#include <QString>

class FileUtils
{
    public:
        static bool copyDirRecursively(const QString &_fromDir, const QString &_toDir);
        static bool removeDirRecursively(const QString &_dir);
        static bool directoryExists(const QString &_dir);
    private:
        FileUtils();
        ~FileUtils();
};

#endif