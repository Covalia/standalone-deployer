#ifndef LOADERMANAGER_H
#define LOADERMANAGER_H

#include <QObject>

class LoaderManager : public QObject
{
    Q_OBJECT

    public:
        LoaderManager();
        virtual ~LoaderManager();

        bool launchUpdater();

        void  closeAppEvent();

    signals:
        void closeAppSignal();
};

#endif // LOADERMANAGER_H
