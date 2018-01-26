#include "tools/factory/macos/macosfactory.h"
#include "tools/path/macos/macospathimpl.h"

MacosFactory::MacosFactory()
{
}

MacosFactory::~MacosFactory()
{
}

PathImpl * MacosFactory::makePath()
{
    // La classe Path est responsable de libérer cette instance
    return new MacosPathImpl();
}
