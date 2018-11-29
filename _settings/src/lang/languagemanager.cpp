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
	L_INFO("System Locale detected: " + locale);
	return locale;
}

void LanguageManager::updateLocale(const QString &_locale)
{
    L_INFO("Language init : locale=" + _locale);
    qApp->removeTranslator(&translator);
    const QString location = ":/translations";

    L_INFO("Loading translator");
    if (translator.load(_locale, location)) {
        L_INFO("Loading translator Finished");
        qApp->installTranslator(&translator);
    } else {
        L_INFO("Could not intall translation " + _locale);
    }
}
