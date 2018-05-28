/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, Antony jr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------------
 *  @filename 	 	: QArchive.hpp
 *  @description 	: C++ Cross-Platform helper library that Modernizes libarchive
 *  			  using Qt5. Simply extracts 7z  , Tarballs  , RAR
 *  			  and other supported formats by libarchive.
 *  @tag		: v0.0.3
 * -----------------------------------------------------------------------------
*/
#if !defined (QARCHIVE_HPP_INCLUDED)
#define QARCHIVE_HPP_INCLUDED

#include <QtCore>

/*
 * Getting the libarchive headers for the
 * runtime operating system
*/
extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
}

namespace QArchive   // QArchive Namespace Start
{
/*
 * QArchive error codes
 * --------------------
*/
enum {
    NO_ARCHIVE_ERROR,
    ARCHIVE_QUALITY_ERROR,
    ARCHIVE_READ_ERROR,
    ARCHIVE_UNCAUGHT_ERROR,
    ARCHIVE_FATAL_ERROR,
    INVALID_DEST_PATH,
    DISK_OPEN_ERROR,
    DISK_READ_ERROR,
    FILE_NOT_EXIST
};


/*
 * Class Extractor <- Inherits QObject.
 * ---------------
 *
 *  Takes care of extraction of archives with the help
 *  of libarchive.
 *
 *  Constructors:
 *  	Extractor(const QString&) 	- Simply extract a single archive , any format
 *  			      	  , Most likely will be used for 7zip.
 *
 *  Methods:
 *	void setArchive(const QString&)     - Set an archive to extract.
 *
 *  Slots:
 *	void start(void)	      - starts the extractor.
 *  Signals:
 *	void finished()		        - emitted when all extraction job is done.
 *	void extracting(const QString&) - emitted with the filename beign extracted.
 *	void extracted(const QString&)  - emitted with the filename that has been extracted.
 *	void status(const QString& , const QString&) - emitted with the entry and the filename on extraction.
 *	void error(short , const QString&) - emitted when something goes wrong!
 *
*/
class Extractor  : public QObject
{
    Q_OBJECT

public:
    explicit Extractor(const QString& filename, const QString& destination)
        : QObject(NULL), dest(cleanDestPath(destination))
    {
        setArchive(filename);
        return;
    }

    void setArchive(const QString& filename)
    {
        if(mutex.tryLock()) {
            file = filename;
            mutex.unlock();
        }
        return;
    }

    void setDestination(const QString& destination)
    {
        if(mutex.tryLock()) {
            dest = cleanDestPath(destination);
            mutex.unlock();
        }
        return;
    }

    ~Extractor()
    {
    }

public slots:

    void start(void)
    {
        if(!mutex.tryLock()) {
            return;
        }
        startExtraction();
    }

signals:
    void finished();
    void extracted(const QString&);
    void extracting(const QString&);
    void status(const QString&, const QString&);
    void error(short, const QString&);

private slots:
    QString cleanDestPath(const QString& input)
    {
        QString ret = QDir::cleanPath(QDir::toNativeSeparators(input));
        if(ret.at(ret.count() - 1) != QDir::separator()) {
            ret += QDir::separator();
        }
        return ret;
    }

    int extract(const char* filename, const char* dest)
    {

        struct archive *arch,*ext;
        struct archive_entry *entry;
        int ret = 0;

        arch = archive_read_new();
        ext = archive_write_disk_new();
        archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME);
        archive_read_support_format_all(arch);
        archive_read_support_filter_all(arch);

        if((ret = archive_read_open_filename(arch, filename, 10240))) {
            return ARCHIVE_READ_ERROR;
        }
        while (true) {
            ret = archive_read_next_header(arch, &entry);
            if (ret == ARCHIVE_EOF) {
                break;
            }
            if (ret != ARCHIVE_OK) {
                return ARCHIVE_QUALITY_ERROR;
            }

            if(dest != NULL) {
                char* new_entry = concat( dest, archive_entry_pathname(entry));
                archive_entry_set_pathname(entry, new_entry);
                emit status(QString(filename), QString(new_entry));
                free(new_entry);
            } else {
                emit status(QString(filename), QString(archive_entry_pathname(entry)));
            }

            ret = archive_write_header(ext, entry);
            if (ret == ARCHIVE_OK) {
                copy_data(arch, ext);
                ret = archive_write_finish_entry(ext);
                if (ret != ARCHIVE_OK) {
                    return ARCHIVE_UNCAUGHT_ERROR;
                }
            }

        }
        archive_read_close(arch);
        archive_read_free(arch);
        archive_write_close(ext);
        archive_write_free(ext);
        return NO_ARCHIVE_ERROR;
    }

    int copy_data(struct archive *arch, struct archive *ext)
    {
        const void *buff;
        size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
        int64_t offset;
#else
        off_t offset;
#endif
        while (true) {
            int ret = archive_read_data_block(arch, &buff, &size, &offset);
            if (ret == ARCHIVE_EOF)
                return (ARCHIVE_OK);
            if (ret != ARCHIVE_OK)
                return (ret);
            ret = archive_write_data_block(ext, buff, size, offset);
            if (ret != ARCHIVE_OK) {
                return (ret);
            }
        }
        return NO_ARCHIVE_ERROR;
    }

    char *concat(const char *dest, const char *src)
    {
        char *ret = (char*) calloc(sizeof(char), strlen(dest) + strlen(src) + 1);

        strcpy(ret, dest);
        strcat(ret, src);

        return ret;
    }

    void startExtraction()
    {
        short error_code = NO_ARCHIVE_ERROR;
        std::string dest_str = dest.toStdString();
        const char *destination = (dest.isEmpty()) ? NULL : dest_str.c_str();

        if(destination != NULL) {
            /*
             * Check if the directory exist!
             */
            if(!QDir(dest).exists()) {
                emit error(INVALID_DEST_PATH, file);
                mutex.unlock();
                return;
            }
        }

        emit extracting(file);
        if( (error_code = extract(file.toStdString().c_str(), destination)) ) {
            emit error(error_code, file);
            mutex.unlock();
            return;
        }
        emit extracted(file);
        mutex.unlock();

        emit finished();
        return;
    }

private:
    QMutex mutex; // thread-safe!
    QString file;
    QString	dest;
}; // Extractor Class Ends

} // QArchive Namespace Ends.
#endif // QARCHIVE_HPP_INCLUDED
