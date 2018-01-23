#ifndef UPDATER__ZIPEXTRACTOR_H
#define UPDATER__ZIPEXTRACTOR_H

#include <QObject>
#include <QString>

namespace QArchive {
class Extractor;
}

class ZipExtractor : QObject {
    Q_OBJECT

    public:
        ZipExtractor(const QString &_zipPath, const QString &_extractDir, QObject * _parent = 0);
        virtual ~ZipExtractor();
        void extract();

    signals:
        void finished();
        void error();

    private slots:
        void extract_finished();
        void extract_error(short _errorCode, const QString &_file);

    private:
        const QString m_extractDir;
        QArchive::Extractor *m_extractor;

};

#endif // ifndef UPDATER__ZIPEXTRACTOR_H
