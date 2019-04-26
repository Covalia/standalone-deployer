#ifndef UPDATER_DOWNLOADMANAGERERROR_H
#define UPDATER_DOWNLOADMANAGERERROR_H

#include <QObject>

class DownloadManagerError {
    Q_GADGET

    public:
        enum ErrorType {
            ProxyAuthenticationError,
            DownloadError,
            TimeoutError
        };
        Q_ENUM(ErrorType)

    private:
        DownloadManagerError()
        {
        }

        ~DownloadManagerError()
        {
        }
};

#endif
