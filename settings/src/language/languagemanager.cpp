#include "language/languagemanager.h"
#include "log/logger.h"

#include <QString>
#include <QLocale>
#include <QDebug>
#include <QCoreApplication>
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

Language LanguageManager::getLanguageFromLocale(QString locale)
{
    if(locale == "en_US"){
        return Language::English;
    }
    else if(locale == "fr_FR"){
        return Language::French;
    }
    else{
        L_WARN("Language witch locale " + locale + " is not supported by application");
        return Language::English;
    }
}
