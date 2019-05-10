#ifndef LOADER_LOADERMANAGER_H
#define LOADER_LOADERMANAGER_H

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

#endif
