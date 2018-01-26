#include "tools/factory/windows/windowsfactory.h"
#include "tools/path/windows/windowspathimpl.h"

WindowsFactory::WindowsFactory() {
}

WindowsFactory::~WindowsFactory() {
}

PathImpl * WindowsFactory::makePath() {
    // La classe Path est responsable de libérer cette instance
    return new WindowsPathImpl();
}
