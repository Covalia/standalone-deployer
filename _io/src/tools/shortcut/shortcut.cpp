#include "tools/shortcut/shortcut.h"
#include "tools/factory/factory.h"

Shortcut::Shortcut() : m_shortcutImpl(0)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_shortcutImpl = factory->makeShortcut();
    }
}

Shortcut::~Shortcut()
{
    delete m_shortcutImpl;
}

bool Shortcut::createDesktopShortcut(InstallPath _installPath, QString _shortcutName, QString _args, QString _installLocation, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createDesktopShortcut(_installPath, _shortcutName, _args, _installLocation, _applicationName);
    }
    return false;
}

bool Shortcut::createStartShorcut(InstallPath _installPath, QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartShorcut(_installPath, _shortcutName, _allUser, _installLocation, _applicationName);
    }
    return false;
}

bool Shortcut::createStartMenuShorcut(InstallPath _installPath, QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartMenuShorcut(_installPath, _startMenuFolderName, _allUser, _installLocation, _applicationName);
    }
    return false;
}
