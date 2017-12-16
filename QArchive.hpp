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
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
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
#include <archive.h>
#include <archive_entry.h>
}

namespace QArchive { // QArchive Namespace Start
/*
 * QArchive error codes
 * --------------------
*/
enum {
    NO_ARCHIVE_ERROR,
    ARCHIVE_QUALITY_ERROR,
    ARCHIVE_READ_ERROR,
    ARCHIVE_UNCAUGHT_ERROR,
    INVALID_DEST_PATH
};


/*
 * Class Extractor <- Inherits QThread
 * ---------------
 *
 *  Takes care of extraction of archives with the help
 *  of libarchive.
 *
 *  Constructors:
 *  	Extractor(QObject *parent = NULL)
 *
 *  	Extractor(const QString&) 	- Simply extract a single archive , any format
 *  			      	  , Most likely will be used for 7zip.
 *  	Extractor(const QStringList&)	- Extract all archives from the QStringList in
 *  				  order.
 *
 *  Methods:
 *	void addArchive(const QString&)     - Add a archive to the queue.
 *	void addArchive(const QStringList&) - Add a set of archives to the queue
 *	void removeArchive(const QString&)  - Removes a archive from the queue matching
 *					the QString.
 *
 *  Slots:
 *	void start(void)	      - starts the extractor. (Inherited from QThread)
 *	void stop(void)		      - stops the extractor.  (Inherited from QThread)
 *
 *  Signals:
 *	void finished()		        - emitted when all extraction job is done.
 *	void extracting(const QString&) - emitted with the filename beign extracted.
 *	void extracted(const QString&)  - emitted with the filename that has been extracted.
 *	void status(const QString& , const QString&) - emitted with the entry and the filename on extraction.
 *	void error(short , const QString&) - emitted when something goes wrong!
 *
*/
class Extractor  : public QThread
{
    Q_OBJECT

public:
    explicit Extractor(QObject *parent = NULL)
    {
    }

    explicit Extractor(const QString& filename)
    {
        addArchive(filename);
    }

    explicit Extractor(const QStringList& filenames)
    {
        addArchive(filenames);
    }

    explicit Extractor(const QString& filename, const QString& destination)
        : dest(cleanDestPath(destination))
    {
        addArchive(filename);
    }

    explicit Extractor(const QStringList& filenames, const QString& destination)
        : dest(cleanDestPath(destination))
    {
        addArchive(filenames);
    }

    void addArchive(const QString& filename)
    {
        queue << filename;
        queue.removeDuplicates();
    }

    void addArchive(const QStringList& filenames)
    {

        /*
         * No need to extract the same archive twice!
        */
        queue << filenames;
        queue.removeDuplicates();
    }

    void removeArchive(const QString& filename)
    {
        queue.removeAll(filename);
    }

    void setDestination(const QString& destination)
    {
        dest = cleanDestPath(destination);
    }

    void run() override {
        short error_code = NO_ARCHIVE_ERROR;
        const char *destination = (dest.isEmpty()) ? NULL : dest.toStdString().c_str();

        if(destination != NULL)
        {
            /*
             * Check if the directory exist!
             */
            if(!QDir(dest).exists())
            {
                emit error(INVALID_DEST_PATH, queue.takeFirst());
                return;
            }
        }

        for(auto i = 0; i < queue.size(); ++i)
        {
            emit extracting(queue.at(i));
            if( (error_code = extract(queue.at(i).toStdString().c_str(), destination)) ) {
                emit error(error_code, queue.at(i));
                queue.removeAll(queue.at(i));
                return;
            }
            emit extracted(queue.at(i));
        }
        queue.clear();
        emit finished();
        return;
    }

    ~Extractor() { }

signals:
    void finished();
    void extracted(const QString&);
    void extracting(const QString&);
    void status(const QString&, const QString&);
    void error(short, const QString&);
private:
    QStringList queue;
    QString	dest;

    QString cleanDestPath(const QString& input)
    {
        QString ret = QDir::cleanPath(QDir::toNativeSeparators(input));
        if(ret.at(ret.count()) != QDir::separator()) {
            ret += QDir::separator();
        }
        return ret;
    }

    int extract(const char* filename, const char* dest) {

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
        for (;;) {
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
        for (int ret = 0;;) {
            ret = archive_read_data_block(arch, &buff, &size, &offset);
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

};

/*
 * Class Compressor <- Inherits QThread.
 * ----------------
 *
 *  Compresses files and folder into a archive.
 *
 *  Constructors:
 *
 *	Compressor(QObject *parent = NULL)
 *
 *	Compressor(const QString& , const QStringList&) - sets an archive with the files from QStringList.
 *	Compressor(const QString& , const QString&) - sets an archive with a single file or folder.
 *      Compressor(const QString&) - only set the archive path to be created.
 *
 *  Methods:
 *	
 *	void setArchive(const QString&)  - sets the archive path to be created.
 *	void addFiles(const QString&)    - add a single file or folder to the archive.
 *	void addFiles(const QStringList&)- add a list of files and folders to the archive.
 *	void removeFiles(const QString&) - removes a file from the archive.
 *	void removeFiles(const QStringList&) - removes a list of files from the archive.
 *
 *  Slots:
 *	void start() - starts or resumes the compression. (Inherited from QThread)
 *	void stop()  - stops or pauses the compression. (Inherited from QThread)
 *
 *  Signals:
 * 	void finished() - Emitted when all jobs are done.
 * 	void compressing(const QString&) - Emitted with the file beign compressed.
 * 	void compressed(const QString&)  - Emitted with file had been compressed.
 * 	void error(short , const QString&) - Emitted with error code refering a file.
 *
*/

class Compressor : public QThread 
{
	Q_OBJECT
public:
	explicit Compressor(QObject *parent = NULL)
	{
	}

	explicit Compressor(const QString& , const QStringList&)
	{
	}

	~Compressor() { }

signals:
	void finished();
	void compressing(const QString&);
	void compressed(const QString&);
	void error(short , const QString&);

private:
	QString archivePath;
	QStringList nodes;

};

} // QArchive Namespace Ends.
#endif // QARCHIVE_HPP_INCLUDED
