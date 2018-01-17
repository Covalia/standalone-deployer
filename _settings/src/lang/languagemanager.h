#ifndef SETTINGS__LANGUAGEMANAGER_H
#define SETTINGS__LANGUAGEMANAGER_H

#include <QTranslator>
#include <QString>

enum Language { English, French };

class LanguageManager
{
    public:
        LanguageManager();

        static void initLanguage();
        static void updateLanguage(QString locale);

        static Language getLanguageFromLocale(QString locale);
        static QString getStringLanguageFromEnum(Language language);

    private:
        static QTranslator translator;
};

#endif // SETTINGS__LANGUAGEMANAGER_H
