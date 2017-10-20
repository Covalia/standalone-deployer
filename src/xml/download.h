#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QString>

class Download
{

public:

    Download(QString _href,
             QString _hashMac,
             QString _os,
             bool _native,
             bool _main);

    QString getHref() const;
    QString getHashMac() const;
    QString getOs() const;
    bool isNative() const;
    bool isMain() const;

    operator QString() const {
        return "Download [href=" + m_href + ", hashMac=" + m_hashMac + ", os=" + m_os
                + ", native=" + QString::number(m_native) + ", main=" + QString::number(m_main) + "]";
    }

private:
    QString m_href;
    QString m_hashMac;
    QString m_os;
    bool m_native;
    bool m_main;

};

#endif
