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

        virtual bool prepareLoader();
        virtual bool prepareUpdater(QString _version);

        virtual bool startLoader(QStringList _args);
        virtual bool startUpdater(QString _version, QStringList _args);

        virtual QString getClasspathSeparator() const;

    protected:
        virtual QString getJavaExecutablePath(const QString &_javaVersion) const;

    private:
        QDir getMountDir();
        QSharedPointer<QFile> getLoaderAppFile();
        QSharedPointer<QFile> getUpdaterAppFile(QString _updaterVersion);
        bool extractAppFromDmgIfNotExist(const QString &_appName, const QFile &_dmgPath, const QFile &_appPath, const QString &_appInDmgPath, bool _forceOverwrite);
        bool openDmgFile(const QFile &_dmgFile);
        bool closeDmgFile(const QFile &_dmgFile);
};

#endif
