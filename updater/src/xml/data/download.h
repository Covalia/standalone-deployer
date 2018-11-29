#ifndef UPDATER__DOWNLOAD_H
#define UPDATER__DOWNLOAD_H

#include <QString>
#include "utils/hashmac/hashmac512.h"

/**
 * @class Download
 * \brief This class represents a file to download into application into CNLP file
 */
class Download
{
    public:

        Download(QString _href,
                 QString _hashMac,
                 QString _os,
                 QString _version,
                 bool _native,
                 bool _inClasspath);

        QString getHref() const;
        HashMacString getHashMac() const;
        QString getOs() const;
        QString getVersion() const;
        bool isNative() const;
        bool isInClasspath() const;

        operator QString() const {
            return "Download [href=" + m_href + ", hashMac=" + m_hashMac.shortHashMac() + ", os=" + m_os
                   + ", version=" + m_version + ", native=" + QString::number(m_native) + ", main=" + QString::number(m_inClasspath) + "]";
        }

    private:
        QString m_href;
        HashMacString m_hashMac;
        QString m_os;
        QString m_version;
        bool m_native;
        bool m_inClasspath;
};

#endif
