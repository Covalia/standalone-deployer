#include "io/unzip/zipextractor.h"
#include "io/unzip/qarchive/qarchive.h"
#include "log/logger.h"

ZipExtractor::ZipExtractor(const QString &_zipPath, const QString &_extractDir, QObject * _parent) :
    QObject(_parent),
    m_extractDir(_extractDir),
    m_extractor(0),
    ok(false)
{
    L_INFO("Initializing extraction of " + _zipPath + " to directory " + _extractDir);

    if (!_zipPath.isEmpty() && _zipPath.endsWith(".zip", Qt::CaseInsensitive)) {
        m_extractor = new QArchive::Extractor(_zipPath, _extractDir);
        connect(m_extractor, SIGNAL(finished()), this, SLOT(extract_success()));

        // emitted when something goes wrong
        connect(m_extractor, SIGNAL(error(short,const QString&)),
                this, SLOT(extract_error(short,const QString&)));
    } else {
        L_ERROR("Archive file does not end with zip.");
    }
}

ZipExtractor::~ZipExtractor()
{
    delete m_extractor;
}

void ZipExtractor::extract()
{
    if (m_extractor) {
        L_INFO("Checking if destination " + m_extractDir + " is an empty directory");
        QDir directory(m_extractDir);

        if (directory.exists()) {
            L_INFO("Java version directory already exists, removing content.");
            directory.removeRecursively();
        }

        if (!directory.exists()) {
            L_INFO("Java version directory does not exist, creating.");
            QDir().mkpath(directory.absolutePath());
        }

        L_INFO("Starting extraction...");
        // start extracting
        m_extractor->start();
    } else {
        L_ERROR("Extractor is not defined.");
        emit error();
    }
} // ZipExtractor::extract

void ZipExtractor::extract_success()
{
    L_INFO("Extraction finished.");
    ok = true;
    emit success();
    emit finished();
}

void ZipExtractor::extract_error(short _errorCode, const QString &_file)
{
    L_INFO("Extraction finished with error code: " + QString::number(_errorCode));

    switch (_errorCode) {
        case QArchive::ARCHIVE_READ_ERROR:
            qDebug() << "unable to find archive :: " << _file;
            break;
        case QArchive::ARCHIVE_QUALITY_ERROR:
            qDebug() << "bad archive! :: " << _file;
            break;
        case QArchive::ARCHIVE_UNCAUGHT_ERROR:
            qDebug() << "fatal error. :: " << _file;
            break;
        default:
            qDebug() << "unknown error. :: " << _file;
            break;
    }

    ok = false;
    emit error();
    emit finished();
} // ZipExtractor::extract_error

bool ZipExtractor::isOk() const {
    return ok;
}
