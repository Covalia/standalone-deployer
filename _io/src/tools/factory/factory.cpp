#include "tools/factory/factory.h"

#include <QtGlobal>

#ifdef Q_OS_WIN
#include "tools/factory/windows/windowsfactory.h"
#endif
#ifdef Q_OS_MACOS
#include "tools/factory/macos/macosfactory.h"
#endif

std::mutex Factory::sm_mutex;

Factory * Factory::getFactory()
{
    static Factory * factory = 0;

    if (!factory) {
#ifdef Q_OS_WIN
            factory = WindowsFactory::getFactoryInstance();
#endif
#ifdef Q_OS_MACOS
            factory = MacosFactory::getFactoryInstance();
#endif
    }
    return factory;
}
