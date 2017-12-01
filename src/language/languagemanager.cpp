#include "language/languagemanager.h"
#include "log/logger.h"

#include <QString>
#include <QLocale>
#include <QDebug>
#include <QApplication>
#include <QTranslator>

QTranslator LanguageManager::translator;

LanguageManager::LanguageManager()
{
}

void LanguageManager::initLanguage()
{
    QString locale = QLocale::system().name();

    updateLanguage(locale);
}

void LanguageManager::updateLanguage(QString locale)
{
    L_INFO("Language init : locale=" + locale);
    qApp->removeTranslator(&translator);
    QString location = ":/translations";

    L_INFO("loading translator");
    if (translator.load(locale, location)) {
        L_INFO("LOAD FINISHED");
        qApp->installTranslator(&translator);
    } else {
        L_INFO("COULD NOT INSTALL TRANSLATIONS " + locale);
    }
}
