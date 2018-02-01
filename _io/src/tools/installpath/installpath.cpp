#include "tools/installpath/installpath.h"
#include "tools/factory/factory.h"

InstallPath::InstallPath() : m_pathInstallImpl(0)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_pathInstallImpl = factory->makeInstallPath();
    }
}

InstallPath::~InstallPath() {
    delete m_pathInstallImpl;
}

QString InstallPath::getInstallationRootPath(FileSystemConfig::AppComponent _app)
{
    if (m_pathInstallImpl) {
        return m_pathInstallImpl->getInstallationRootPath(_app);
    }
    return "";
}
