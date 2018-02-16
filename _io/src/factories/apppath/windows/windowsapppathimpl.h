#ifndef IO__WINDOWSAPPPATHIMPL_H
#define IO__WINDOWSAPPPATHIMPL_H

#include "factories/apppath/apppathimpl.h"

class WindowsAppPathImpl : public AppPathImpl {
    public:
        WindowsAppPathImpl(FileSystemConfig::AppComponent _app);
        virtual bool makeAppDirectories();
        virtual QString getLoaderResourcesPath();
        virtual QString getUpdaterResourcesPath();
        virtual QString getLoaderFilePath();
        virtual QString getUpdaterFilePath(QString _updaterVersion);

    protected:
        virtual bool startApplication(QString _app, QStringList _args);
};

#endif
