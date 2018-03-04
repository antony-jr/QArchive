/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2018, Antony jr
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
 *  @filename 	 	: QArchive.cc
 *  @description 	: C++ Cross-Platform helper library that Modernizes libarchive
 *  			  using Qt5. Simply extracts 7z  , Tarballs  , RAR
 *  			  and other supported formats by libarchive.
 *  			  The Source file.
 * -----------------------------------------------------------------------------
*/
#include "../include/QArchive.hpp"

/*
 * QArchive::Extractor.
 * -----------------------
 *  Extractor Class Source.
 * ----------------------------
 * Constructor and Destructor.
 * ----------------------------
*/
QArchive::Extractor::Extractor(QObject *parent)
    : QObject(parent)
{
    return;
}

QArchive::Extractor::Extractor(const QString& filename)
    : QObject(nullptr)
{
    addArchive(filename);
    return;
}

QArchive::Extractor::Extractor(const QStringList& filenames)
    : QObject(nullptr)
{
    addArchive(filenames);
    return;
}

QArchive::Extractor::Extractor(const QString& filename, const QString& destination)
    : QObject(nullptr), dest(cleanDestPath(destination))
{
    addArchive(filename);
    return;
}

QArchive::Extractor::Extractor(const QStringList& filenames, const QString& destination)
    : QObject(nullptr), dest(cleanDestPath(destination))
{
    addArchive(filenames);
    return;
}

QArchive::Extractor::~Extractor()
{
    if(Promise != nullptr) {
        stop();
    }
    return;
}

/* ------ */

/*
 * Public Methods.
 * ---------------
 *  QArchive::Extractor
*/
void QArchive::Extractor::addArchive(const QString& filename)
{
    if(mutex.tryLock()) {
        queue << filename;
        queue.removeDuplicates();
        mutex.unlock();
    }
    return;
}

void QArchive::Extractor::addArchive(const QStringList& filenames)
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

void QArchive::Extractor::removeArchive(const QString& filename)
{
    if(mutex.tryLock()) {
        queue.removeAll(filename);
        mutex.unlock();
    }
    return;
}

void QArchive::Extractor::setDestination(const QString& destination)
{
    if(mutex.tryLock()) {
        dest = cleanDestPath(destination);
        mutex.unlock();
    }
    return;
}

/* ------ */

/*
 * Public Slots.
 * ---------------
 *  QArchive::Extractor
*/
bool QArchive::Extractor::isRunning() const
{
    return Promise->isRunning();
}

void QArchive::Extractor::start(void)
{
    if(!mutex.tryLock()) {
        return;
    }
    Promise = new QFuture<void>;
    *Promise = QtConcurrent::run(this, &Extractor::startExtraction);
    return;
}

void QArchive::Extractor::stop(void)
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

/* ------ */

/*
 * Private Slots.
 * ---------------
 *  QArchive::Extractor
 *  Internals for the extractor.
*/
QString QArchive::Extractor::cleanDestPath(const QString& input)
{
    QString ret = QDir::cleanPath(QDir::toNativeSeparators(input));
    if(ret.at(ret.count() - 1) != QDir::separator()) {
        ret += QDir::separator();
    }
    return ret;
}

int QArchive::Extractor::extract(const char* filename, const char* dest)
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

int QArchive::Extractor::copy_data(struct archive *arch, struct archive *ext)
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

char *QArchive::Extractor::concat(const char *dest, const char *src)
{
    char *ret = (char*) calloc(sizeof(char), strlen(dest) + strlen(src) + 1);

    strcpy(ret, dest);
    strcat(ret, src);

    return ret;
}

void QArchive::Extractor::startExtraction()
{
    short error_code = NO_ARCHIVE_ERROR;
    const char *destination = (dest.isEmpty()) ? NULL : dest.toStdString().c_str();

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

/* ------ //
 *  Class Extractor Ends Here.
 * -----------------------------
*/


/*
 * Class QArchive::Compressor
 * --------------------------
 *  QArchive Compressor Source
 * -----------------------------
 * Constructor and Destructor.
 * -----------------------------
*/
QArchive::Compressor::Compressor(QObject *parent)
    : QObject(parent)
{
    return;
}

QArchive::Compressor::Compressor(const QString& archive)
    : QObject(nullptr)
{
    setArchive(archive);
    return;
}

QArchive::Compressor::Compressor(const QString& archive, const QStringList& files)
    : QObject(nullptr)
{
    setArchive(archive);
    addFiles(files);
    return;

}

QArchive::Compressor::Compressor(const QString& archive, const QString& file)
    : QObject(nullptr)
{
    setArchive(archive);;
    addFiles(file);
    return;
}

QArchive::Compressor::~Compressor()
{
    if(Promise != nullptr) {
        stop(); // Just in case.
    }
    return;
}

/* ------ */

/*
 * Public Methods.
 * -----------------
 *  QArchive::Compressor.
*/
void QArchive::Compressor::setArchive(const QString& archive)
{
    if(mutex.tryLock()) {
        archivePath = QDir::cleanPath(archive);
        mutex.unlock();
    }
    return;
}

void QArchive::Compressor::setArchiveFormat(short type)
{
    if(mutex.tryLock()) {
        archiveFormat = type;
        mutex.unlock();
    }
    return;
}

void QArchive::Compressor::addFiles(const QString& file)
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

void QArchive::Compressor::addFiles(const QStringList& files)
{
    if(mutex.tryLock()) {
        nodes << files;
        nodes.removeDuplicates();
        mutex.unlock();
    }
    return;
}

void QArchive::Compressor::removeFiles(const QString& file)
{
    if(mutex.tryLock()) {
        if(!nodes.isEmpty()) {
            nodes.removeAll(file);
        }
        mutex.unlock();
    }
    return;
}

void QArchive::Compressor::removeFiles(const QStringList& files)
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

/* ------ */


/*
 * Public Slots.
 * -------------
 *  QArchive::Compressor
*/
bool QArchive::Compressor::isRunning() const
{
    return Promise->isRunning();
}

void QArchive::Compressor::start(void)
{
    if(!mutex.tryLock()) {
        return;
    }
    Promise = new QFuture<void>;
    *Promise = QtConcurrent::run(this, &Compressor::startCompression);
    return;
}

void QArchive::Compressor::stop(void)
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


/* ------ */

/*
 * Private Slots.
 * ---------------
 *  QArchive::Compressor
*/
void QArchive::Compressor::checkNodes()
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

void QArchive::Compressor::getArchiveFormat()
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

void QArchive::Compressor::startCompression()
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

/* ------ //
 *  Class Compressor Ends Here.
 * -------------------------------
*/


/*
 * Class QArchive::Reader.
 * ------------------------
 *  QArchive Reader Source.
 * ---------------------------
 * Constructor and Destructor.
 * ---------------------------
*/
QArchive::Reader::Reader(QObject *parent)
    : QObject(parent)
{
    return;
}
QArchive::Reader::Reader(const QString& archive)
    : QObject(nullptr)
{
    setArchive(archive);
    return;
}

QArchive::Reader::~Reader()
{
    if(Promise != nullptr) {
        stop();
    }
    return;
}

/* ------ */

/*
 * Public Method.
 * --------------
 *  QArchive::Reader
*/
void QArchive::Reader::setArchive(const QString& archive)
{
    if(mutex.tryLock()) {
        Archive = QDir::cleanPath(archive);
        mutex.unlock();
    }
    return;
}

const QStringList& QArchive::Reader::listFiles()
{
    return Files;
}

void QArchive::Reader::clear()
{
    if(mutex.tryLock()) {
        Archive.clear();
        Files.clear();
        mutex.unlock();
    }
    return;
}

/* ------ */

/*
 * Public Slots.
 * --------------
 *  QArchive::Reader
*/
bool QArchive::Reader::isRunning() const
{
    return Promise->isRunning();
}

void QArchive::Reader::start(void)
{
    if(!mutex.tryLock()) {
        return;
    }
    Promise = new QFuture<void>;
    *Promise = QtConcurrent::run(this, &Reader::startReading);
    return;
}

void QArchive::Reader::stop(void)
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

/* ------ */

/*
 * Private Slots.
 * ---------------
 *  QArchive::Reader
*/
void QArchive::Reader::startReading()
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

/* ------ //
 *  Class QArchive::Reader Ends Here.
 * ------------------------------------
*/

// =============================================================== //
