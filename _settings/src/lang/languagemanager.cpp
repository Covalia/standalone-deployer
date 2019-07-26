#include "lang/languagemanager.h"
#include "log/logger.h"

#include <QString>
#include <QLocale>
#include <QDebug>
#include <QCoreApplication>
#include <QTranslator>
#include <QLibraryInfo>

QTranslator LanguageManager::sm_appTranslator;
QTranslator LanguageManager::sm_qtTranslator;

QString LanguageManager::getSystemLocale()
{
    const QString locale = QLocale::system().name();

    L_INFO(QString("System Locale detected: %1").arg(locale));
    return locale;
}

void LanguageManager::updateLocale(const QString &_locale)
{
    L_INFO(QString("Language init locale: %1").arg(_locale));

    // remove old translators
    qApp->removeTranslator(&sm_appTranslator);
    qApp->removeTranslator(&sm_qtTranslator);

    const QString appTranslationsPath = ":/translations/app";
    const QString qtTranslationsPath = ":/translations/qt";

    L_INFO("Loading app translator");
    if (sm_appTranslator.load(_locale, appTranslationsPath)) {
        L_INFO("Loading app translator Finished");
        qApp->installTranslator(&sm_appTranslator);
    } else {
        L_WARN(QString("Could not intall app translation: %1").arg(_locale));
    }

    L_INFO("Loading qt translator");
    if (sm_qtTranslator.load("qtbase_" + _locale, qtTranslationsPath)) {
        L_INFO("Loading qt translator Finished");
        qApp->installTranslator(&sm_qtTranslator);
    } else {
        L_WARN(QString("Could not intall qt translation: %1").arg(_locale));
    }
}
