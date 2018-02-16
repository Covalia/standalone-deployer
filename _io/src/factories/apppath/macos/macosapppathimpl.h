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
        virtual QString getUpdaterFilePath(QString _updaterVersion);

        virtual bool startLoader(QStringList _args);
        virtual bool startUpdater(QString _version, QStringList _args);

    protected:
        virtual bool startApplication(QString _app, QStringList _args);

    private:
        QDir getMountDirPath();
        QString getLoaderAppFilePath();
        QString getUpdaterAppFilePath(QString updaterVersion);
        bool extractAppFromDmgIfNotExist(const QString &_appName, const QString &_dmgPath, const QString &_appPath, const QString &_appInDmgPath);
        bool openDmgFile(const QString &_dmgFile);
        bool closeDmgFile(const QString &_dmgFile);
};

#endif
