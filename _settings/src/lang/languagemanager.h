#ifndef SETTINGS__LANGUAGEMANAGER_H
#define SETTINGS__LANGUAGEMANAGER_H

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

#endif // SETTINGS__LANGUAGEMANAGER_H
