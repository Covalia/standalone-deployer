#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QTranslator>

class LanguageManager
{
public:
    LanguageManager();

    static void initLanguage();
    static void updateLanguage(QString locale);

private:
    static QTranslator translator;
};

#endif // LANGUAGEMANAGER_H
