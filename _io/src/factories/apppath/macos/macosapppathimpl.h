#ifndef IO__MACOSAPPPATHIMPL_H
#define IO__MACOSAPPPATHIMPL_H

#include "factories/apppath/apppathimpl.h"

class MacosAppPathImpl : public AppPathImpl {
    public:
        MacosAppPathImpl(FileSystemConfig::AppComponent _app);
        virtual bool makeAppDirectories();
        virtual QString getLoaderResourcesPath();
        virtual QString getUpdaterResourcesPath();
        virtual QString getLoaderFilePath();
        virtual QString getUpdaterFilePath(QString updaterVersion);

    protected:
        virtual bool startApplication(QString _app, QStringList _args);
};

#endif
