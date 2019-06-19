#include "factories/osresources/osresources.h"
#include "factories/factory/factory.h"

#ifdef Q_OS_WIN
#include "factories/osresources/windows/windowsresourcesimpl.h"
#endif
#ifdef Q_OS_MACOS
#include "factories/osresources/macos/macosresourcesimpl.h"
#endif

OsResources::OsResources(const QString &_installPath) : m_osResourcesImpl(nullptr)
{
    Factory * factory = Factory::getFactory();

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
