#ifndef CONFIG__GLOBAL_H
#define CONFIG__GLOBAL_H

#include <QString>

class Global
{

	public:
		/// URL de l'application
		static const QString AppUrl;

		/// Chemin de l'installation
		static const QString InstallationDir;

		/// Chemin relatif pour trouver le fichier cnlp
		static const QString AppCnlpRelativePath;

		/// Chemin relatif d'installation de l'application
		static const QString AppDir;

		/// Chemin relatif de la configuration de l'application
		static const QString ConfigurationDir;

		/// Chemin relatif des extensions de l'application
		static const QString ExtensionDir;

		/// Chemin relatif de Java
		static const QString JavaDir;

		/// Chemin relatif des logs de l'application
		static const QString LogsDir;

		/// Chemin relatif du répertoire temp de l'application
		static const QString TempDir;

};

#endif
