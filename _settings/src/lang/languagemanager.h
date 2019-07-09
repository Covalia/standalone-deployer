#ifndef SETTINGS_LANGUAGEMANAGER_H
#define SETTINGS_LANGUAGEMANAGER_H

#include <QTranslator>
#include <QString>

class LanguageManager
{
    public:

        static QString getSystemLocale();
        static void updateLocale(const QString &_locale);

    private:
        LanguageManager();
        ~LanguageManager();

        // the application translator
        static QTranslator sm_appTranslator;

        // the qt translator: translate standard buttons, etc.
        static QTranslator sm_qtTranslator;
};

#endif
