#ifndef IO__MACOSAPPPATHIMPL_H
#define IO__MACOSAPPPATHIMPL_H

#include "factories/apppath/apppathimpl.h"

class MacosAppPathImpl : public AppPathImpl {
    public:
        MacosAppPathImpl(IOConfig::AppComponent _app);
        virtual bool makeAppDirectories();
        virtual QSharedPointer<QFile> getLoaderResourcesFile();
        virtual QSharedPointer<QFile> getUpdaterResourcesFile();
        virtual QSharedPointer<QFile> getLoaderFile();
        virtual QSharedPointer<QFile> getUpdaterFile(QString _updaterVersion);

        virtual bool startLoader(QStringList _args);
        virtual bool startUpdater(QString _version, QStringList _args);

    protected:
        virtual bool startApplication(QSharedPointer<QFile> _app, QStringList _args);

    private:
        QDir getMountDir();
        QSharedPointer<QFile> getLoaderAppFile();
        QSharedPointer<QFile> getUpdaterAppFile(QString _updaterVersion);
        bool extractAppFromDmgIfNotExist(const QString &_appName, const QFile &_dmgPath, const QFile &_appPath, const QString &_appInDmgPath);
        bool openDmgFile(const QFile &_dmgFile);
        bool closeDmgFile(const QFile &_dmgFile);
};

#endif