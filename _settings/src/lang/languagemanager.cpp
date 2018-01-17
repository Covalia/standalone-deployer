#include "lang/languagemanager.h"
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

    L_INFO("Loading translator");
    if (translator.load(locale, location)) {
        L_INFO("Loading translator Finished");
        qApp->installTranslator(&translator);
    } else {
        L_INFO("Could not intall translations " + locale);
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

QString LanguageManager::getStringLanguageFromEnum(Language language){
     L_INFO("LOAD FINISHED");
    switch(language){
        case English :
            return "English";
        case French :
            return "Francais";
        default :
            return "Default";
    }
}
