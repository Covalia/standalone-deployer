#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QString>

class Download
{



public:
    Download( QString _href,
              QString _hashMac,
              QString _os,
              bool _native,
              bool _main);

    QString toString();

    QString getHref() const;
    void setHref(const QString &href);

    QString getHashMac() const;
    void setHashMac(const QString &hashMac);

    QString getOs() const;
    void setOs(const QString &os);

    bool getNative() const;
    void setNative(bool native);

    bool getMain() const;
    void setMain(bool main);

private:
    QString m_href;
    QString m_hashMac;
    QString m_os;
    bool m_native;
    bool m_main;
};

#endif // DOWNLOAD_H
