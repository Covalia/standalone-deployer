#include "download.h"

Download::Download(QString _href,
                   QString _hashMac,
                   QString _os,
                   bool _native,
                   bool _main) {
    m_href = _href;
    m_hashMac = _hashMac;
    m_os = _os;
    m_native = _native;
    m_main = _main ;
}

QString Download::getHref() const
{
    return m_href;
}

void Download::setHref(const QString &href)
{
    m_href = href;
}

QString Download::getHashMac() const
{
    return m_hashMac;
}

void Download::setHashMac(const QString &hashMac)
{
    m_hashMac = hashMac;
}

QString Download::getOs() const
{
    return m_os;
}

void Download::setOs(const QString &os)
{
    m_os = os;
}

bool Download::getNative() const
{
    return m_native;
}

void Download::setNative(bool native)
{
    m_native = native;
}

bool Download::getMain() const
{
    return m_main;
}

void Download::setMain(bool main)
{
    m_main = main;
}

QString Download::toString(){
    return "Download [href=" + m_href + ", hashMac=" + m_hashMac + ", os=" + m_os
            + ", native_=" + m_native + ", main=" + m_main + "]";
}


