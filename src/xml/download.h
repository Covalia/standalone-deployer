#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QString>

class Download
{
    QString href;
    QString hashMac;
    QString os;
    bool native;
    bool main;

public:
    Download( QString _href,
              QString _hashMac,
              QString _os,
              bool _native,
              bool _main);

    QString toString();
};

#endif // DOWNLOAD_H
