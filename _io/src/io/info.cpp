#include "io/info.h"
#include "build_hash.h"

QString Info::getBuildHash()
{
    return QString(STANDALONE_DEPLOYER_BUILD_HASH);
}
