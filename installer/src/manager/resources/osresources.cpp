#include "manager/resources/osresources.h"

bool OsResources::extractProjectIniToTempFile(const QString &_toPath)
{
#ifdef Q_OS_WIN
        return WindowsResources::extractProjectIniToTempFile(_toPath);
#endif
#ifdef Q_OS_MACOS
        return MacosResources::extractProjectIniToTempFile(_toPath);
#endif
}

bool OsResources::extractStyleCssToTempFile(const QString &_toPath)
{
#ifdef Q_OS_WIN
        return WindowsResources::extractStyleCssToTempFile(_toPath);
#endif
#ifdef Q_OS_MACOS
        return MacosResources::extractStyleCssToTempFile(_toPath);
#endif
}

bool OsResources::extractTitlePngToTempFile(const QString &_toPath)
{
#ifdef Q_OS_WIN
        return WindowsResources::extractTitlePngToTempFile(_toPath);
#endif
#ifdef Q_OS_MACOS
        return MacosResources::extractTitlePngToTempFile(_toPath);
#endif
}
