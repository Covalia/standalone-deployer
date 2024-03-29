#include "installerfactories/shortcut/shortcut.h"
#include "installerfactories/factory/installerfactory.h"

Shortcut::Shortcut() : m_shortcutImpl(nullptr)
{
    InstallerFactory * factory = InstallerFactory::getFactory();

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

bool Shortcut::createStartShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartShortcut(_appPath, _shortcutName, _applicationName);
    }
    return false;
}

bool Shortcut::createStartMenuShortcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName)
{
    if (m_shortcutImpl) {
        return m_shortcutImpl->createStartMenuShortcut(_appPath, _startMenuFolderName, _applicationName);
    }
    return false;
}
