#ifndef IO_ZIPEXTRACTOR_H
#define IO_ZIPEXTRACTOR_H

#include <QObject>
#include <QString>

namespace QArchive {
class Extractor;
}

class ZipExtractor : public QObject {
    Q_OBJECT

    public:
        ZipExtractor(const QString &_zipPath, const QString &_extractDir, QObject * _parent = 0);
        virtual ~ZipExtractor();
        void extract();
        bool isOk() const;

    signals:
        void finished(); // called by error and success
        void error();
        void success();

    private slots:
        void extract_success();
        void extract_error(short _errorCode, const QString &_file);

    private:
        const QString m_extractDir;
        QArchive::Extractor *m_extractor;
        bool ok;

};

#endif
