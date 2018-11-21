#include "xml/data/download.h"

Download::Download(QString _href,
                   QString _hashMac,
                   QString _os,
                   QString _version,
                   bool _native,
                   bool _inClasspath) :
    m_href(_href),
    m_hashMac(_hashMac),
    m_os(_os),
    m_version(_version),
    m_native(_native),
    m_inClasspath(_inClasspath)
{
}

QString Download::getHref() const
{
    return m_href;
}

HashMacString Download::getHashMac() const
{
    return m_hashMac;
}

QString Download::getOs() const
{
    return m_os;
}

QString Download::getVersion() const
{
    return m_version;
}

bool Download::isNative() const
{
    return m_native;
}

bool Download::isInClasspath() const
{
    return m_inClasspath;
}
