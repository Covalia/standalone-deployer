#include "lang/languagemanager.h"
#include "log/logger.h"

#include <QString>
#include <QLocale>
#include <QDebug>
#include <QCoreApplication>
#include <QTranslator>

QTranslator LanguageManager::translator;

QString LanguageManager::getSystemLocale()
{
	const QString locale = QLocale::system().name();
	L_INFO(QString("System Locale detected: %1").arg(locale));
	return locale;
}

void LanguageManager::updateLocale(const QString &_locale)
{
    L_INFO(QString("Language init locale: %1").arg(_locale));
    qApp->removeTranslator(&translator);
    const QString location = ":/translations";

    L_INFO("Loading translator");
    if (translator.load(_locale, location)) {
        L_INFO("Loading translator Finished");
        qApp->installTranslator(&translator);
    } else {
		L_INFO(QString("Could not intall translation: %1").arg(_locale));
    }
}
