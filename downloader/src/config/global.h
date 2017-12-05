#ifndef CONFIG__GLOBAL_H
#define CONFIG__GLOBAL_H

#include <QString>

class Global
{
public:

    static const QString AppName;
    static const QString AppCnlpRemoteFilename;
    static const QString AppCnlpLocalFilename;

    static const QString StarterName;
    static const QString StarterCnlpRemoteFilename;
    static const QString StarterCnlpLocalFilename;

    static const QString DownloaderName;
    static const QString DownloaderCnlpRemoteFilename;
    static const QString DownloaderCnlpLocalFilename;
};

#endif // ifndef CONFIG__GLOBAL_H
