#ifndef IO_WINDOWSAPPPATHIMPL_H
#define IO_WINDOWSAPPPATHIMPL_H

#include "factories/apppath/apppathimpl.h"

class WindowsAppPathImpl : public AppPathImpl {
    public:
        WindowsAppPathImpl(IOConfig::AppComponent _app);
        virtual bool makeAppDirectories();
        virtual QSharedPointer<QFile> getLoaderResourcesFile();
        virtual QSharedPointer<QFile> getUpdaterResourcesFile();
        virtual QSharedPointer<QFile> getLoaderFile();
        virtual QSharedPointer<QFile> getUpdaterFile(QString _updaterVersion);

        virtual bool startLoader(QStringList _args);
        virtual bool startUpdater(QString _version, QStringList _args);

        virtual QString getClasspathSeparator() const;

    protected:
        virtual QString getJavaExecutablePath(const QString &_javaVersion) const;
};

#endif
