#include "installerfactories/factory/installerfactory.h"

#include <QtGlobal>

#ifdef Q_OS_WIN
#include "installerfactories/factory/windows/windowsinstallerfactory.h"
#endif
#ifdef Q_OS_MACOS
#include "installerfactories/factory/macos/macosinstallerfactory.h"
#endif

std::mutex InstallerFactory::sm_mutex;

InstallerFactory * InstallerFactory::getFactory()
{
    static InstallerFactory * factory = nullptr;

    if (!factory) {
#ifdef Q_OS_WIN
            factory = WindowsInstallerFactory::getFactoryInstance();
#endif
#ifdef Q_OS_MACOS
            factory = MacosInstallerFactory::getFactoryInstance();
#endif
    }
    return factory;
}
