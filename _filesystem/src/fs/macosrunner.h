#ifndef FILESYSTEM__MACOSRUNNER_H
#define FILESYSTEM__MACOSRUNNER_H

#include <QObject>
#include <QString>

class MacosRunner : public QObject {
    Q_OBJECT

    public:
        static bool openDmgFile(const QString &_appName);
        static bool closeDmgFile(const QString &_appName);
};

#endif
