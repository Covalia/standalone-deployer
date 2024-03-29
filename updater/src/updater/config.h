#ifndef UPDATER_CONFIG_H
#define UPDATER_CONFIG_H

#include <QString>

class UpdaterConfig
{
    public:
        // TODO inutile
        /// Chemin de l'installation
        static const QString InstallationDir;

        /// Updater User-Agent.
        static const QString UserAgentValue;

        /// Remote deploy root directory.
        static const QString DeployRootPath;

        /// Remote application cnlp filename.
        static const QString AppCnlpRemoteFilename;
        /// Local application cnlp filename.
        static const QString AppCnlpLocalFilename;

        /// Remote loader cnlp filename.
        static const QString LoaderCnlpRemoteFilename;
        /// Local loader cnlp filename.
        static const QString LoaderCnlpLocalFilename;

        /// Remote updater cnlp filename.
        static const QString UpdaterCnlpRemoteFilename;
        /// Local updater cnlp filename.
        static const QString UpdaterCnlpLocalFilename;

        /// Remote java cnlp filename.
        static const QString JavaCnlpRemoteFilename;
        /// Local java cnlp filename.
        static const QString JavaCnlpLocalFilename;
};

#endif
