#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QTranslator>

enum Language { English, French };

class LanguageManager
{
    public:
        LanguageManager();

        static void initLanguage();
        static void updateLanguage(QString locale);

        static Language getLanguageFromLocal(QString locale);

    private:
        static QTranslator translator;
};

#endif // LANGUAGEMANAGER_H
