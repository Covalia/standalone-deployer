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

bool Shortcut::createDesktopShortcut(QString _shortcutName, QString _args, QString _installLocation, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createDesktopShortcut(_shortcutName, _args, _installLocation, _applicationName);
    }
    return false;
}

bool Shortcut::createStartShorcut(QString _shortcutName, bool _allUser, QString _installLocation, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartShorcut(_shortcutName, _allUser, _installLocation, _applicationName);
    }
    return false;
}

bool Shortcut::createStartMenuShorcut(QString _startMenuFolderName, bool _allUser, QString _installLocation, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartMenuShorcut(_startMenuFolderName, _allUser, _installLocation, _applicationName);
    }
    return false;
}
