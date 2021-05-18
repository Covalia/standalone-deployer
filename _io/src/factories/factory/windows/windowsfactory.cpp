#include "factories/factory/windows/windowsfactory.h"
#include "factories/apppath/windows/windowsapppathimpl.h"

WindowsFactory::WindowsFactory()
{
}

WindowsFactory::~WindowsFactory()
{
}

AppPathImpl * WindowsFactory::makeAppPath(IOConfig::AppComponent _app)
{
    // La classe Path est responsable de libérer cette instance
    return new WindowsAppPathImpl(_app);
}
