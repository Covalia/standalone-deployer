#include "installerfactories/osresources/osresources.h"
#include "installerfactories/factory/installerfactory.h"

#ifdef Q_OS_WIN
#include "installerfactories/osresources/windows/windowsresourcesimpl.h"
#endif
#ifdef Q_OS_MACOS
#include "installerfactories/osresources/macos/macosresourcesimpl.h"
#endif

OsResources::OsResources(const QString &_installPath) : m_osResourcesImpl(nullptr)
{
    InstallerFactory * factory = InstallerFactory::getFactory();

    if (factory) {
        m_osResourcesImpl = QSharedPointer<OsResourcesImpl>(factory->makeOsResources(_installPath));
    }
}

OsResources::~OsResources()
{
}

bool OsResources::extractResources()
{
    if (m_osResourcesImpl) {
        return m_osResourcesImpl->extractResources();
    }
    return false;
}

bool OsResources::extractProjectIniToTempFile(const QString &_toPath)
{
#ifdef Q_OS_WIN
        return WindowsResourcesImpl::extractProjectIniToTempFile(_toPath);
#endif
#ifdef Q_OS_MACOS
        return MacosResourcesImpl::extractProjectIniToTempFile(_toPath);
#endif
}

bool OsResources::extractStyleCssToTempFile(const QString &_toPath)
{
#ifdef Q_OS_WIN
        return WindowsResourcesImpl::extractStyleCssToTempFile(_toPath);
#endif
#ifdef Q_OS_MACOS
        return MacosResourcesImpl::extractStyleCssToTempFile(_toPath);
#endif
}

bool OsResources::extractTitlePngToTempFile(const QString &_toPath)
{
#ifdef Q_OS_WIN
        return WindowsResourcesImpl::extractTitlePngToTempFile(_toPath);
#endif
#ifdef Q_OS_MACOS
        return MacosResourcesImpl::extractTitlePngToTempFile(_toPath);
#endif
}

bool OsResources::extractShortcutIcnsIconToTempFile(const QString &_fromResource, const QString &_toPath)
{
#ifdef Q_OS_WIN
        return WindowsResourcesImpl::extractShortcutIcnsIconToTempFile(_fromResource, _toPath);
#endif
#ifdef Q_OS_MACOS
        return MacosResourcesImpl::extractShortcutIcnsIconToTempFile(_fromResource, _toPath);
#endif
}
