#ifndef INSTALLER_ABSTRACT_TRANSLATED_UI_H
#define INSTALLER_ABSTRACT_TRANSLATED_UI_H

#include <QApplication>

class AbstractTranslatedUi {
    public:

        virtual void retranslateUi() = 0;

        QString translate_helper(const QString &_context, const QString &_translateStr) const
        {
            std::string translateStdStr = _translateStr.toStdString();
            std::string contextStdStr = _context.toStdString();

            return QApplication::translate(contextStdStr.c_str(), translateStdStr.c_str(), Q_NULLPTR);
        }

        QString tr_helper(const QString &_translateStr) const
        {
            std::string str = _translateStr.toStdString();

            return QObject::tr(str.c_str());
        }
};

#endif
