#ifndef IO__WINDOWSAPPPATHIMPL_H
#define IO__WINDOWSAPPPATHIMPL_H

#include "factories/apppath/apppathimpl.h"

class WindowsAppPathImpl : public AppPathImpl {
    public:
        WindowsAppPathImpl(IOConfig::AppComponent _app);
        virtual bool makeAppDirectories();
        virtual QSharedPointer<QFile> getLoaderResourcesFile();
        virtual QSharedPointer<QFile> getUpdaterResourcesFile();
        virtual QSharedPointer<QFile> getLoaderFile();
        virtual QSharedPointer<QFile> getUpdaterFile(QString _updaterVersion);

    protected:
        virtual bool startComponent(QSharedPointer<QFile> _app, QStringList _args);
        virtual QString getJavaExecutablePath(const QString &_javaVersion) const;
};

#endif
