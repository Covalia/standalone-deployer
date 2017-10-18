#include "download.h"

Download::Download(QString _href,
                   QString _hashMac,
                   QString _os,
                   bool _native,
                   bool _main) {
    href = _href;
    hashMac = _hashMac;
    os = _os;
    native = _native;
    main = _main;
}

QString Download::toString(){
    return "Download [href=" + href + ", hashMac=" + hashMac + ", os=" + os
            + ", native_=" + native + ", main=" + main + "]";
}


