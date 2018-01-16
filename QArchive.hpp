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
#include <QtConcurrentRun>

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
 *	void start(void)	      - starts the extractor.
 *	void stop(void)		      - stops the extractor.
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
    explicit Extractor(QObject *parent = NULL)
        : QObject(parent)
    {
        return;
    }

    explicit Extractor(const QString& filename)
        : QObject(NULL)
    {
        addArchive(filename);
        return;
    }

    explicit Extractor(const QStringList& filenames)
        : QObject(NULL)
    {
        addArchive(filenames);
        return;
    }

    explicit Extractor(const QString& filename, const QString& destination)
        : QObject(NULL), dest(cleanDestPath(destination))
    {
        addArchive(filename);
        return;
    }

    explicit Extractor(const QStringList& filenames, const QString& destination)
        : QObject(NULL), dest(cleanDestPath(destination))
    {
        addArchive(filenames);
        return;
    }

    void addArchive(const QString& filename)
    {
        if(mutex.tryLock()) {
            queue << filename;
            queue.removeDuplicates();
            mutex.unlock();
        }
        return;
    }

    void addArchive(const QStringList& filenames)
    {

        /*
         * No need to extract the same archive twice!
        */
        if(mutex.tryLock()) {
            queue << filenames;
            queue.removeDuplicates();
            mutex.unlock();
        }
        return;
    }

    void removeArchive(const QString& filename)
    {
        if(mutex.tryLock()) {
            queue.removeAll(filename);
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
        if(Promise != nullptr) {
            stop();
        }
        return;
    }

public slots:
    bool isRunning() const
    {
        return Promise->isRunning();
    }

    void start(void)
    {
        if(!mutex.tryLock()) {
            return;
        }
        Promise = new QFuture<void>;
        *Promise = QtConcurrent::run(this, &Extractor::startExtraction);
        return;
    }

    void stop(void)
    {
        /*
         * If mutex is not locked then that means
         * there is no start called or the operation is
         * finished , so doing stop is useless.
         */
        if(mutex.tryLock() && !Promise->isRunning()) {
            mutex.unlock();
            return;
        }
        stopExtraction = true;
        return;
    }

signals:
    void stopped();
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
        for (; !stopExtraction;) {
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
        for (int ret = 0; !stopExtraction;) {
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
                emit error(INVALID_DEST_PATH, queue.takeFirst());
                mutex.unlock();
                return;
            }
        }

        for(auto i = 0; i < queue.size() && !stopExtraction; ++i) {
            emit extracting(queue.at(i));
            if( (error_code = extract(queue.at(i).toStdString().c_str(), destination)) ) {
                emit error(error_code, queue.at(i));
                queue.removeAll(queue.at(i));
                mutex.unlock();
                return;
            }
            if(!stopExtraction) {
                emit extracted(queue.at(i));
            }
        }
        mutex.unlock();
        queue.clear();
        if(stopExtraction) {
            emit(stopped());
            return;
        }
        emit finished();
        return;
    }

private:
    bool stopExtraction = false; // stop flag!
    QMutex mutex; // thread-safe!
    QStringList queue;
    QString	dest;
    QFuture<void> *Promise = nullptr; // Promise suits this good than future!
}; // Extractor Class Ends

/*
 * Supported Archive Types for Compressor
 * --------------------------------------
*/
enum {
    NO_FORMAT,
    BZIP,
    BZIP2,
    CPIO,
    GZIP,
    RAR,
    ZIP,
    SEVEN_ZIP
};

/*
 * Class Compressor <- Inherits QObject.
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
 *  	Compressor(const QString&) - only set the archive path to be created.
 *
 *  Methods:
 *
 *	void setArchive(const QString&)  - sets the archive path to be created.
 *	void setArchiveFormat(short) - sets the archive format.
 *	void addFiles(const QString&)    - add a single file or folder to the archive.
 *	void addFiles(const QStringList&)- add a list of files and folders to the archive.
 *	void removeFiles(const QString&) - removes a file from the archive.
 *	void removeFiles(const QStringList&) - removes a list of files from the archive.
 *
 *  Slots:
 *	void start() - starts the compression.
 *	void stop()  - stops the compression.
 *
 *  Signals:
 *      void stopped() - Emitted when the process is stopped successfully.
 * 	void finished() - Emitted when all jobs are done.
 * 	void compressing(const QString&) - Emitted with the file beign compressed.
 * 	void compressed(const QString&)  - Emitted with file had been compressed.
 * 	void error(short , const QString&) - Emitted with error code refering a file.
 *
*/

class Compressor : public QObject
{
    Q_OBJECT
public:
    explicit Compressor(QObject *parent = NULL)
        : QObject(parent)
    {
        return;
    }

    explicit Compressor(const QString& archive)
        : QObject(NULL)
    {
        setArchive(archive);
        return;
    }

    explicit Compressor(const QString& archive, const QStringList& files)
        : QObject(NULL)
    {
        setArchive(archive);
        addFiles(files);
        return;

    }

    explicit Compressor(const QString& archive, const QString& file)
        : QObject(NULL)
    {
        setArchive(archive);;
        addFiles(file);
        return;
    }

    void setArchive(const QString& archive)
    {
        if(mutex.tryLock()) {
            archivePath = QDir::cleanPath(archive);
            mutex.unlock();
        }
        return;
    }

    void setArchiveFormat(short type)
    {
        if(mutex.tryLock()) {
            archiveFormat = type;
            mutex.unlock();
        }
        return;
    }

    void addFiles(const QString& file)
    {
        /*
         * No like files can exist in a filesystem!
        */
        if(mutex.tryLock()) {
            nodes << file;
            nodes.removeDuplicates();
            mutex.unlock();
        }
        return;
    }

    void addFiles(const QStringList& files)
    {
        if(mutex.tryLock()) {
            nodes << files;
            nodes.removeDuplicates();
            mutex.unlock();
        }
        return;
    }

    void removeFiles(const QString& file)
    {
        if(mutex.tryLock()) {
            if(!nodes.isEmpty()) {
                nodes.removeAll(file);
            }
            mutex.unlock();
        }
        return;
    }

    void removeFiles(const QStringList& files)
    {
        if(mutex.tryLock()) {
            if(!nodes.isEmpty()) {
                for(QStringListIterator filesIt(files); filesIt.hasNext();) {
                    removeFiles(filesIt.next());
                }
            }
            mutex.unlock();
        }
        return;
    }

    ~Compressor()
    {
        if(Promise != nullptr) {
            stop(); // Just in case.
        }
        return;
    }

public slots:

    bool isRunning() const
    {
        return Promise->isRunning();
    }

    void start(void)
    {
        if(!mutex.tryLock()) {
            return;
        }
        Promise = new QFuture<void>;
        *Promise = QtConcurrent::run(this, &Compressor::startCompression);
        return;
    }

    void stop(void)
    {
        /*
         * If mutex is not locked then that means
         * there is no start called or the operation is
         * finished , so doing stop is useless.
         */
        if(mutex.tryLock() && !Promise->isRunning()) {
            mutex.unlock();
            return;
        }
        stopCompression = true;
        return;
    }



private slots:

    /*
     * Checks if the files are valid , use only on runtime!
    */
    void checkNodes()
    {
        for(QStringListIterator nodeIt(nodes); nodeIt.hasNext();) {
            QString currentNode(nodeIt.next());
            QFileInfo fInfo(currentNode);
            if(!fInfo.exists()) {
                nodes.removeAll(currentNode);
                emit error(FILE_NOT_EXIST, currentNode);
            }
        }
    }

    /*
     * Automatically finds the file format from archive extension
    */
    void getArchiveFormat()
    {
        QFileInfo fInfo(archivePath);
        QString ext = fInfo.suffix();

        if(ext.toLower() == "bz") {
            archiveFormat = BZIP;
        }
        if(ext.toLower() == "bz2") {
            archiveFormat = BZIP2;
        } else if(ext.toLower() == "gz") {
            archiveFormat = GZIP;
        } else if(ext.toLower() == "cpio") {
            archiveFormat = CPIO;
        } else if(ext.toLower() == "rar") {
            archiveFormat = RAR;
        } else if(ext.toLower() == "zip") {
            archiveFormat = ZIP;
        } else if(ext.toLower() == "7z") {
            archiveFormat = SEVEN_ZIP;
        } else {
            archiveFormat = NO_FORMAT; // default
        }
    }

    void startCompression()
    {
        checkNodes(); // clear unwanted files
        qDebug() << nodes;
        if(nodes.isEmpty() || archivePath.isEmpty()) {
            mutex.unlock();
            return;
        }
        if(archiveFormat == NO_FORMAT) {
            getArchiveFormat();
        }
        // Creating the archive!
        char buff[16384];
        struct archive *a;
        struct archive_entry *entry;
        ssize_t len;
        bool noTar = false;
        int fd;

        a = archive_write_new();
        switch (archiveFormat) {
        case BZIP:
        case BZIP2:
        case NO_FORMAT:
            archive_write_add_filter_bzip2(a);
            break;
        case GZIP:
            archive_write_add_filter_gzip(a);
            break;
        default:
            noTar = true;
            archive_write_add_filter_none(a);
            break;
        }
        if(noTar) {
            if(archiveFormat == SEVEN_ZIP) {
                archive_write_set_format_7zip(a);
            }
            if(archiveFormat == RAR) {
                archive_write_set_format_xar(a);
            }
            if(archiveFormat == ZIP) {
                archive_write_set_format_zip(a);
            }
        } else {
            archive_write_set_format_ustar(a);
        }
        archive_write_open_filename(a, archivePath.toStdString().c_str());

        QStringListIterator nodeIt(nodes);
        while (nodeIt.hasNext() && !stopCompression) {
            struct archive *disk = archive_read_disk_new();
            archive_read_disk_set_standard_lookup(disk);
            QString currentNode = nodeIt.next();
            int r;

            emit compressing(currentNode);

            r = archive_read_disk_open(disk, currentNode.toStdString().c_str());
            if (r != ARCHIVE_OK) {
                emit error(DISK_OPEN_ERROR, currentNode);
                mutex.unlock();
                return;
            }

            for (; !stopCompression;) {
                entry = archive_entry_new();
                r = archive_read_next_header2(disk, entry);
                if (r == ARCHIVE_EOF)
                    break;
                if (r != ARCHIVE_OK) {
                    emit error(DISK_READ_ERROR, currentNode);
                    mutex.unlock();
                    return;
                }
                archive_read_disk_descend(disk);
                r = archive_write_header(a, entry);

                if (r == ARCHIVE_FATAL) {
                    emit error(ARCHIVE_FATAL_ERROR, currentNode);
                    mutex.unlock();
                    return;
                }
                if (r > ARCHIVE_FAILED) {
                    /* For now, we use a simpler loop to copy data
                     * into the target archive. */
                    fd = open(archive_entry_sourcepath(entry), O_RDONLY);
                    len = read(fd, buff, sizeof(buff));
                    while (len > 0 && !stopCompression) {
                        archive_write_data(a, buff, len);
                        len = read(fd, buff, sizeof(buff));
                    }
                    close(fd);
                }
                archive_entry_free(entry);
            }
            archive_read_close(disk);
            archive_read_free(disk);
            if(!stopCompression) {
                emit compressed(currentNode);
            }
        }
        archive_write_close(a);
        archive_write_free(a);
        if(stopCompression) {
            mutex.unlock();
            emit(stopped());
            return;
        }
        nodes.clear();
        mutex.unlock();
        emit finished();
        return;
    }

signals:
    void stopped();
    void finished();
    void compressing(const QString&);
    void compressed(const QString&);
    void error(short, const QString&);
private:
    bool stopCompression = false;
    QMutex mutex;
    QFuture<void> *Promise = nullptr;
    QString archivePath;
    QStringList nodes;
    short archiveFormat = NO_FORMAT; // Default
}; // Compressor Class Ends

/*
 * Class Reader <- Inherits QObject.
 * ------------
 *
 * Gets the list of files inside a archive.
 *
 * Constructors:
 * 	Reader(QObject *parent = NULL) - Only Constructs the reader.
 * 	Reader(const QString&) - Sets a single archive and Constructs the reader.
 *
 * Methods:
 *
 * 	void setArchive(const QString&) - Sets a single archive
 *	void clear()			- Clears everything stored in this class.
 *	const QStringList& listFiles() - get the files stored in this class.
 *
 * Slots:
 * 	start() - Starts the operation.
 * 	stop()  - Stops the operation.
 *
 * Signals:
 *      void stopped() - Emitted when stop() is successfull.
 *	void error(short , const QString&) - Emitted when something goes wrong.
 * 	void archiveFiles(const QString& , const QStringList&) - Emitted when we got all the files from the archive.
*/

class Reader : public QObject
{
    Q_OBJECT
public:
    explicit Reader(QObject *parent = NULL)
        : QObject(parent)
    {
        return;
    }
    explicit Reader(const QString& archive)
        : QObject(NULL)
    {
        setArchive(archive);
        return;
    }

    void setArchive(const QString& archive)
    {
        if(mutex.tryLock()) {
            Archive = QDir::cleanPath(archive);
            mutex.unlock();
        }
        return;
    }

    const QStringList& listFiles()
    {
        return Files;
    }

    void clear()
    {
        if(mutex.tryLock()) {
            Archive.clear();
            Files.clear();
            mutex.unlock();
        }
        return;
    }

    ~Reader()
    {
        if(Promise != nullptr) {
            stop();
        }
        return;
    }

public slots:
    bool isRunning() const
    {
        return Promise->isRunning();
    }

    void start(void)
    {
        if(!mutex.tryLock()) {
            return;
        }
        Promise = new QFuture<void>;
        *Promise = QtConcurrent::run(this, &Reader::startReading);
        return;
    }

    void stop(void)
    {
        /*
         * If mutex is not locked then that means
         * there is no start called or the operation is
         * finished , so doing stop is useless.
         */
        if(mutex.tryLock() && !Promise->isRunning()) {
            mutex.unlock();
            return;
        }
        stopReader = true;
        return;
    }


private slots:
    void startReading()
    {
        if(Archive.isEmpty()) {
            mutex.unlock();
            return;
        }

        QFileInfo fInfo(Archive);
        if(!fInfo.exists()) {
            mutex.unlock();
            emit error(ARCHIVE_READ_ERROR, Archive);
            return;
        }

        struct archive *arch;
        struct archive_entry *entry;
        int ret = 0;

        arch = archive_read_new();
        archive_read_support_format_all(arch);
        archive_read_support_filter_all(arch);

        if((ret = archive_read_open_filename(arch, Archive.toStdString().c_str(), 10240))) {
            mutex.unlock();
            emit error(ARCHIVE_READ_ERROR, Archive);
            return;
        }
        for (; !stopReader;) {
            ret = archive_read_next_header(arch, &entry);
            if (ret == ARCHIVE_EOF) {
                break;
            }
            if (ret != ARCHIVE_OK) {
                mutex.unlock();
                emit error(ARCHIVE_QUALITY_ERROR, Archive);
                return;
            }
            Files << archive_entry_pathname(entry);
        }
        archive_read_close(arch);
        archive_read_free(arch);
        mutex.unlock();
        if(stopReader) {
            emit(stopped());
            return;
        }
        emit archiveFiles(Archive, Files);
        return;
    }
signals:
    void stopped(void);
    void archiveFiles(const QString&, const QStringList&);
    void error(short, const QString&);

private:
    bool stopReader = false;
    QMutex mutex;
    QString Archive;
    QStringList Files;
    QFuture<void> *Promise = nullptr;
}; // Class Reader Ends

} // QArchive Namespace Ends.
#endif // QARCHIVE_HPP_INCLUDED
