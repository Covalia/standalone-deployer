#include "apptreemanager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include "config/global.h"


AppTreeManager::AppTreeManager(const QString &_installationDir, QObject *_parent) :
	QObject(_parent),
	m_installationDir(_installationDir)
{
	qDebug() << "installation directory" << m_installationDir;
}

AppTreeManager::~AppTreeManager() {
}

bool AppTreeManager::makeAppDirectories() {
	bool result = true;

	result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir + "/" + Global::AppDir);
	result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir + "/" + Global::ConfigurationDir);
	result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir + "/" + Global::ExtensionDir);
	result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir + "/" + Global::JavaDir);
	result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir + "/" + Global::LogsDir);
	result &= AppTreeManager::makeDirectoryIfNotExists(m_installationDir + "/" + Global::TempDir);

	return result;
}

bool AppTreeManager::makeDirectoryIfNotExists(const QString &_directoryPath) {

	if (QFile(_directoryPath).exists()) {
		// si le répertoire est en réalité un fichier, pas bon, il faut nettoyer
		// TODO check les répertoires parent ??
		QFile(_directoryPath).remove();
	}

	if (!QDir(_directoryPath).exists()) {
		return QDir().mkpath(_directoryPath);
	}
	else {
		return true;
	}

}

QString AppTreeManager::getAppDirPath() {
	return m_installationDir + "/" + Global::AppDir;
}

QString AppTreeManager::getConfigurationDirPath() {
	return m_installationDir + "/" + Global::ConfigurationDir;
}

QString AppTreeManager::getExtensionDirPath() {
	return m_installationDir + "/" + Global::ExtensionDir;
}

QString AppTreeManager::getJavaDirPath() {
	return m_installationDir + "/" + Global::JavaDir;
}

QString AppTreeManager::getLogsDirPath() {
	return m_installationDir + "/" + Global::LogsDir;
}

QString AppTreeManager::getTempDirPath() {
    return m_installationDir + "/" + Global::TempDir;
}


