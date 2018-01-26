#include "tools/path/macos/macospathimpl.h"

MacosPathImpl::MacosPathImpl() {
}

MacosPathImpl::~MacosPathImpl() {
}

// TODO implémenter et ajouter des arguments pour gérer chaque application
QString MacosPathImpl::getInstallationRootPath() {
    return QCoreApplication::applicationDirPath();
}
