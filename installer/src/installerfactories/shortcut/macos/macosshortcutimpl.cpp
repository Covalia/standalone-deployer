#include "installerfactories/shortcut/macos/macosshortcutimpl.h"

#include "io/config.h"
#include "installerfactories/osresources/osresources.h"

MacosShortcutImpl::MacosShortcutImpl()
{
}

MacosShortcutImpl::~MacosShortcutImpl()
{
}

bool MacosShortcutImpl::createDesktopShortcut(AppPath _appPath, QString _shortcutName, QString _args, QString _applicationName)
{
    Q_UNUSED(_appPath)
    Q_UNUSED(_shortcutName)
    Q_UNUSED(_args)
    Q_UNUSED(_applicationName)
    // TODO implement
    return true;
}

bool MacosShortcutImpl::createStartShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName)
{
    Q_UNUSED(_appPath)
    Q_UNUSED(_shortcutName)
    Q_UNUSED(_applicationName)
    // TODO implement
    return true;
}

bool MacosShortcutImpl::createStartMenuShortcut(AppPath _appPath, QString _startMenuFolderName, QString _applicationName)
{
    Q_UNUSED(_startMenuFolderName)

    // on windows, used to create shortcut in application directory first, then copy them into start menu folder.
    // on macos, there isn't any start menu folder, so, to be consistent, we create application shortcut here.

    bool configureShortcutCreated = createApplicationShortcut(_appPath, QObject::tr("Configure %1").arg(_applicationName), "configure");
    bool uninstallShortcutCreated = createApplicationShortcut(_appPath, QObject::tr("Uninstall %1").arg(_applicationName), "uninstall");
    bool startShortcutCreated = createApplicationShortcut(_appPath, _applicationName, "start");

    return configureShortcutCreated && uninstallShortcutCreated && startShortcutCreated;
}

bool MacosShortcutImpl::createApplicationShortcut(AppPath _appPath, QString _shortcutName, QString _applicationName)
{
    const QDir shortcutDir(_appPath.getInstallationDir().absoluteFilePath(_shortcutName + IOConfig::MacOsAppExtension));
    const QDir contentsDir(shortcutDir.absoluteFilePath("Contents"));
    const QDir macosDir(contentsDir.absoluteFilePath("MacOS"));
    const QDir resourcesDir(contentsDir.absoluteFilePath("Resources"));

    bool createdDirectories = true;

    if (!macosDir.exists()) {
        createdDirectories &= QDir().mkpath(macosDir.absolutePath());
        L_ERROR(QString("Unable to create directory: %1").arg(macosDir.absolutePath()));
    }
    if (!resourcesDir.exists()) {
        createdDirectories &= QDir().mkpath(resourcesDir.absolutePath());
        L_ERROR(QString("Unable to create directory: %1").arg(resourcesDir.absolutePath()));
    }

    if (createdDirectories) {
        if (_applicationName == "configure" || _applicationName == "uninstall" || _applicationName == "start") {
            QSharedPointer<QFile> plistFileIn(new QFile(QString(":/application/%1/Info.plist").arg(_applicationName)));
            QSharedPointer<QFile> scriptFileIn(new QFile(QString(":/application/%1/%1").arg(_applicationName)));
            const QString icnsIconIn = QString("application/%1/%1.icns").arg(_applicationName);

            QSharedPointer<QFile> plistFileOut(new QFile(QString("%1/Info.plist").arg(contentsDir.absolutePath())));
            QSharedPointer<QFile> scriptFileOut(new QFile(QString("%1/%2").arg(macosDir.absolutePath()).arg(_applicationName)));
            const QString icnsIconOut = QString("%1/%2.icns").arg(resourcesDir.absolutePath()).arg(_applicationName);

            bool extractionOk = true;

            // copy info.plist into contents
            QPair<bool, QString> extractPlist = _appPath.extractResource(*plistFileIn, *plistFileOut);
            if (extractPlist.first) {
                L_INFO(extractPlist.second);
            } else {
                extractionOk = false;
                L_ERROR(extractPlist.second);
            }

            // copy script into macos
            QPair<bool, QString> extractScript = _appPath.extractResource(*scriptFileIn, *scriptFileOut);
            if (extractScript.first) {
                L_INFO(extractScript.second);
            } else {
                extractionOk = false;
                L_ERROR(extractScript.second);
            }

            bool setPermOk = scriptFileOut->setPermissions(
                QFileDevice::ReadOwner // readable by the owner of the file
                | QFileDevice::WriteOwner // writable by the owner of the file
                | QFileDevice::ExeOwner // executable by the owner of the file
                | QFileDevice::ReadGroup // readable by the group
                | QFileDevice::ExeGroup // executable by the group
                | QFileDevice::ReadOther // readable by anyone
                | QFileDevice::ExeOther // executable by anyone
                );

            if (setPermOk) {
                L_INFO(QString("Permissions set on script: %1").arg(scriptFileOut->fileName()));
            } else {
                L_ERROR(QString("Unable to set permissions on script: %1").arg(scriptFileOut->fileName()));
            }

            // copy _applicationName.icns into resources
            if (OsResources::extractShortcutIcnsIconToTempFile(icnsIconIn, icnsIconOut)) {
                L_INFO(QString("Icns Icon for %1 application extracted").arg(_applicationName));
            } else {
                extractionOk = false;
                L_ERROR(QString("Unable to extract Icns Icon for %1 application").arg(_applicationName));
            }

            return extractionOk && setPermOk;
        }
        L_ERROR(QString("Unable to create shortcut, given application name is not in allowed list: %1").arg(_applicationName));
    }

    return false;
}
