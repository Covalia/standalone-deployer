#include "factories/shortcut/shortcut.h"
#include "factories/factory/factory.h"

Shortcut::Shortcut() : m_shortcutImpl(0)
{
    Factory * factory = Factory::getFactory();

    if (factory) {
        m_shortcutImpl = QSharedPointer<ShortcutImpl>(factory->makeShortcut());
    }
}

Shortcut::~Shortcut()
{
}

bool Shortcut::createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createDesktopShortcut(_appPath, _shortcutName, _args, _applicationName);
    }
    return false;
}

bool Shortcut::createStartShorcut(AppPath _appPath, QString _shortcutName, bool _allUser, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartShorcut(_appPath, _shortcutName, _allUser, _applicationName);
    }
    return false;
}

bool Shortcut::createStartMenuShorcut(AppPath _appPath, QString _startMenuFolderName, bool _allUser, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartMenuShorcut(_appPath, _startMenuFolderName, _allUser, _applicationName);
    }
    return false;
}
