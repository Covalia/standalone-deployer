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

        static QTranslator translator;
};

#endif
