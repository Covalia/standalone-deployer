#ifndef APPTREEMANAGER_H
#define APPTREEMANAGER_H

#include <QObject>

class AppTreeManager: public QObject
{
	Q_OBJECT

	public:
		AppTreeManager(QString _installationDir, QObject *_parent = 0);
		virtual ~AppTreeManager();

		bool makeAppDirectories();
		static bool makeDirectoryIfNotExists(QString _directoryPath);

		QString getAppDirPath();
		QString getConfigurationDirPath();
		QString getExtensionDirPath();
		QString getJavaDirPath();
		QString getLogsDirPath();
		QString getTempDirPath();

	private:
		const QString m_installationDir;
};

#endif
