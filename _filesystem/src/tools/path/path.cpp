#include "tools/path/path.h"
#include "tools/factory/factory.h"

Path::Path() : m_pathImpl(0)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_pathImpl = factory->makePath();
    }
}

Path::~Path() {
    delete m_pathImpl;
}

QString Path::getInstallationRootPath()
{
    if (m_pathImpl) {
        return m_pathImpl->getInstallationRootPath();
    }
    return "";
}
