#include "io/unzip/zipextractor.h"
#include "io/unzip/qarchive/qarchive.h"
#include "log/logger.h"

ZipExtractor::ZipExtractor(const QString &_zipPath, const QString &_extractDir, QObject * _parent) :
    QObject(_parent),
    m_extractDir(_extractDir),
    m_extractor(0),
    ok(false)
{
    L_INFO(QString("Initializing extraction of %1 to directory %2").arg(_zipPath).arg(_extractDir));

    if (!_zipPath.isEmpty() && (_zipPath.toLower().endsWith(".zip", Qt::CaseInsensitive)
                                || _zipPath.toLower().endsWith(".jar", Qt::CaseInsensitive))) {
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
        L_INFO(QString("Checking if destination %1 is an empty directory").arg(m_extractDir));
        QDir directory(m_extractDir);

        if (directory.exists()) {
            L_INFO("Java version directory already exists, removing content.");
            if (directory.removeRecursively()) {
				L_INFO("Directory successfully removed.");
			} else {
				L_WARN("Error while removing directory.");
			}
        }

        if (!directory.exists()) {
            L_INFO("Java version directory does not exist, creating.");
            if (QDir().mkpath(directory.absolutePath())) {
				L_INFO("Directory successfully created.");
			} else {
				L_ERROR("Error while creating directory.");
			}
        }

        L_INFO("Starting extraction...");
        // start extracting
        m_extractor->start();
    } else {
        L_ERROR("Extractor is not defined.");
        emit error();
    }
}

void ZipExtractor::extract_success()
{
    L_INFO("Extraction finished.");
    ok = true;
    emit success();
    emit finished();
}

void ZipExtractor::extract_error(short _errorCode, const QString &_file)
{
    L_INFO(QString("Extraction finished with error code: %1").arg(QString::number(_errorCode)));

    switch (_errorCode) {
        case QArchive::ARCHIVE_READ_ERROR:
            L_ERROR(QString("unable to find archive: %1").arg(_file));
            break;
        case QArchive::ARCHIVE_QUALITY_ERROR:
            L_ERROR(QString("bad archive: %1").arg(_file));
            break;
        case QArchive::ARCHIVE_UNCAUGHT_ERROR:
            L_ERROR(QString("fatal error: %1").arg(_file));
            break;
        default:
            L_ERROR(QString("unknown error: %1").arg(_file));
            break;
    }

    ok = false;
    emit error();
    emit finished();
}

bool ZipExtractor::isOk() const
{
    return ok;
}
