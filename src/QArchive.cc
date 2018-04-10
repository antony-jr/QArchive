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
 * Destructors for struct archive.
 * Which is Handled by smart pointers.
*/
static void deleteArchiveReader(struct archive *ar)
{
    archive_read_close(ar);
    archive_read_free(ar);
    return;
}

static void deleteArchiveWriter(struct archive *aw)
{
    archive_write_close(aw);
    archive_write_free(aw);
    return;
}

// ---


using namespace QArchive;


/*
 * Extractor Class Source.
 * ----------------------------
 * Constructor and Destructor.
 * ----------------------------
*/
Extractor::Extractor(QObject *parent)
    : QObject(parent)
{
    connectWatcher();
    return;
}

Extractor::Extractor(const QString &filename)
    : QObject(nullptr)
{
    connectWatcher();
    addArchive(filename);
    return;
}

Extractor::Extractor(const QStringList &filenames)
    : QObject(nullptr)
{
    connectWatcher();
    addArchive(filenames);
    return;
}

Extractor::Extractor(const QString &filename, const QString &destination)
    : QObject(nullptr)
{
    connectWatcher();
    addArchive(filename, destination);
    return;
}

Extractor::Extractor(const QStringList& filenames, const QString& destination)
    : QObject(nullptr)
{
    connectWatcher();
    addArchive(filenames, destination);
    return;
}

Extractor::~Extractor()
{
    if(isRunning()) {
        cancel();
        watcher.waitForFinished(); // block any-thread until every
        // job is stopped successfully ,
        // Otherwise this can cause a
        // segfault.
    }
    return;
}

/* ------ */

/*
 * Public Methods.
 * ---------------
 *  Extractor
*/
Extractor &Extractor::addArchive(const QString &filename)
{
    QMutexLocker locker(&mutex);
    if(!queue.contains(filename)) { // Do not add duplicate filenames.
        queue.insert(filename, QString());
    }
    return *this;
}

Extractor &Extractor::addArchive(const QString &filename, const QString &destination)
{
    QMutexLocker locker(&mutex);
    if(!queue.contains(filename)) { // Do not add duplicate filenames.
        queue.insert(filename, cleanDestPath(destination));
    }
    return *this;
}

Extractor &Extractor::addArchive(const QStringList& filenames)
{
    QMutexLocker locker(&mutex);
    /*
     * We can sync here since this process
     * will be done in a blinding speed that
     * not even the gui will get disturbed.
    */
    Q_FOREACH(QString filename, filenames) {
        // Note: Do not use addArchive function
        // because anything with mutex called
        // frequently can be dangerous.

        if(!queue.contains(filename)) { // Do not add duplicate filenames.
            queue.insert(filename, QString());
        }
    }
    return *this;
}

Extractor &Extractor::addArchive(const QStringList& filenames, const QString &destination)
{
    QMutexLocker locker(&mutex);
    Q_FOREACH(QString filename, filenames) {
        if(!queue.contains(filename)) { // Do not add duplicate filenames.
            queue.insert(filename, cleanDestPath(destination));
        }
    }
    return *this;
}

Extractor &Extractor::removeArchive(const QString &filename)
{
    QMutexLocker locker(&mutex);
    queue.remove(filename);
    return *this;
}

Extractor &Extractor::removeArchive(const QStringList &filenames)
{
    QMutexLocker locker(&mutex);
    Q_FOREACH(QString filename, filenames) {
        queue.remove(filename);
    }
    return *this;
}

Extractor &Extractor::setDefaultDestination(const QString& destination)
{
    QMutexLocker locker(&mutex);
    defaultDestination = cleanDestPath(destination);
    return *this;
}

/* ------ */

/*
 * Public Slots.
 * ---------------
 *  Extractor
*/
Extractor &Extractor::start(void)
{
    if(isPaused()) {
        // If the user called this to
        // resume by mistake.
        resume();
        return *this;
    }
    mutex.lock(); // Block other calls.

    // Copy the keys to the heap or else
    // this will cause a segfault.
    QList<QString> *keys = new QList<QString>;
    QList<QString> copy = queue.uniqueKeys();
    keys->swap(copy);
    // ---

    QFuture<void> *future = new QFuture<void>;
    *future = QtConcurrent::map(*keys,[&](QString key) {
        extract(key, queue.value(key));
        queue.remove(key);
        return;
    });
    watcher.setFuture(*future);
    return *this;
}

Extractor &Extractor::pause(void)
{
    watcher.pause();
    return *this;
}

Extractor &Extractor::resume(void)
{
    watcher.resume();
    return *this;
}

Extractor &Extractor::cancel(void)
{
    watcher.cancel();
    return *this;
}

bool Extractor::isRunning() const
{
    return watcher.isRunning();
}
bool Extractor::isCanceled() const
{
    return watcher.isCanceled();
}
bool Extractor::isPaused() const
{
    return watcher.isPaused();
}
bool Extractor::isStarted() const
{
    return watcher.isStarted();
}

Extractor &Extractor::setFunc(short signal, std::function<void(void)> function)
{
    switch(signal) {
    case STARTED:
        connect(this, &Extractor::started, function);
        break;
    case FINISHED:
        connect(this, &Extractor::finished, function);
        break;
    case PAUSED:
        connect(this, &Extractor::paused, function);
        break;
    case RESUMED:
        connect(this, &Extractor::resumed, function);
        break;
    case CANCELED:
        connect(this, &Extractor::canceled, function);
        break;
    default:
        break;
    };
    return *this;
}

Extractor &Extractor::setFunc(short signal, std::function<void(QString)> function)
{
    if(signal == EXTRACTED) {
        connect(this, &Extractor::extracted, function);
    } else {
        connect(this, &Extractor::extracting, function);
    }
    return *this;
}

Extractor &Extractor::setFunc(std::function<void(QString,QString)> function)
{
    connect(this, &Extractor::status, function);
    return *this;
}

Extractor &Extractor::setFunc(std::function<void(short,QString)> function)
{
    connect(this, &Extractor::error, function);
    return *this;
}

Extractor &Extractor::setFunc(std::function<void(int)> function)
{
    connect(this, &Extractor::progress, function);
    return *this;
}

/* ------ */

/*
 * Private Slots.
 * ---------------
 *  Extractor
 *  Internals for the extractor.
*/

void Extractor::connectWatcher(void)
{
    /*
     * Note: Signal-Signal is possible.
    */
    connect(&watcher, &QFutureWatcher<void>::started, this, &Extractor::started);
    connect(&watcher, &QFutureWatcher<void>::paused, this, &Extractor::paused);
    connect(&watcher, &QFutureWatcher<void>::resumed, this, &Extractor::resumed);
    connect(&watcher, &QFutureWatcher<void>::canceled, this, &Extractor::handleCanceled);
    connect(&watcher, &QFutureWatcher<void>::finished, this, &Extractor::handleFinished);
    connect(&watcher, &QFutureWatcher<void>::progressValueChanged, this, &Extractor::progress);
    return;
}
// -----

/*
 * Special Handles.
*/
void Extractor::handleFinished(void)
{
    /*
     * Remember the lock on
     * 'start' slot.
    */
    mutex.unlock();
    emit(finished());
    return;
}
void Extractor::handleCanceled(void)
{
    /*
     * Unlock the lock that
     * was put on 'start'
     * slot.
    */
    mutex.unlock();
    emit(canceled());
    return;
}
// -----


QString Extractor::cleanDestPath(const QString& input)
{
    QString ret = QDir::cleanPath(QDir::toNativeSeparators(input));
    if(ret.at(ret.count() - 1) != QDir::separator()) {
        ret += QDir::separator();
    }
    return ret;
}

void Extractor::extract(QString file,QString dest)
{
    // Check if the file and the destination folder exists
    QFileInfo check_file(file);
    QFileInfo check_dir(dest);
    // check if file exists and if yes then is it really a file and no directory?
    if (!check_file.exists() || !check_file.isFile()) {
        emit(error(ARCHIVE_READ_ERROR, file));
        return;
    } else {
        if(!dest.isEmpty()) {
            if(!check_dir.exists() || check_dir.isFile()) {
                emit(error(DESTINATION_NOT_FOUND, file));
                return;
            }
        }
    }
    // ---

    // Deriving the destination folder.
    const char *filename = file.toStdString().c_str();
    const char *got_dest = (dest.isEmpty()) ? NULL : dest.toStdString().c_str();
    const char *default_dest = (defaultDestination.isEmpty()) ? NULL : defaultDestination.toStdString().c_str();
    char *final_dest = (char*)((got_dest != NULL) ? got_dest :
                               (default_dest != NULL) ? default_dest : NULL);
    // ---

    int ret = NO_ARCHIVE_ERROR;

    // Signal that we are extracting this archive.
    emit(extracting(file));
    // ---

    QSharedPointer<struct archive> arch = QSharedPointer<struct archive>(archive_read_new(), deleteArchiveReader);
    QSharedPointer<struct archive> ext = QSharedPointer<struct archive>(archive_write_disk_new(), deleteArchiveWriter);
    struct archive_entry *entry;
    archive_write_disk_set_options(ext.data(), ARCHIVE_EXTRACT_TIME);
    archive_read_support_format_all(arch.data());
    archive_read_support_filter_all(arch.data());

    if((ret = archive_read_open_filename(arch.data(), filename, 10240))) {
        emit(error(ARCHIVE_READ_ERROR, file));
        return;
    }
    for (;;) {
        ret = archive_read_next_header(arch.data(), &entry);
        if (ret == ARCHIVE_EOF) {
            break;
        }
        if (ret != ARCHIVE_OK) {
            emit(error(ARCHIVE_QUALITY_ERROR, file));
            return;
        }

        if(final_dest != NULL) {
            char* new_entry = concat(final_dest, archive_entry_pathname(entry));
            archive_entry_set_pathname(entry, new_entry);

            // Status Signal
            emit status(QString(filename), QString(new_entry));
            free(new_entry);
        } else {
            // Status Signal.
            emit status(QString(filename), QString(archive_entry_pathname(entry)));
        }

        ret = archive_write_header(ext.data(), entry);
        if (ret == ARCHIVE_OK) {
            {
                const void *buff;
                size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
                int64_t offset;
#else
                off_t offset;
#endif
                for (ret = 0;;) {
                    ret = archive_read_data_block(arch.data(), &buff, &size, &offset);
                    if (ret == ARCHIVE_EOF) {
                        break;
                    } else if (ret != ARCHIVE_OK) {
                        break;
                    } else {
                        ret = archive_write_data_block(ext.data(), buff, size, offset);
                        if (ret != ARCHIVE_OK) {
                            break;
                        }
                    }
                }
            }
            ret = archive_write_finish_entry(ext.data());
            if (ret != ARCHIVE_OK) {
                emit(error(ARCHIVE_UNCAUGHT_ERROR, file));
                return;
            }
        }

    }

    // Signal that we extracted this archive.
    emit(extracted(file));
    // ------
    return;
}

char *Extractor::concat(const char *dest, const char *src)
{
    char *ret = (char*) calloc(sizeof(char), strlen(dest) + strlen(src) + 1);

    strcpy(ret, dest);
    strcat(ret, src);

    return ret;
}

/* ------ //
 *  Class Extractor Ends Here.
 * -----------------------------
*/


/*
 * Class Compressor
 * --------------------------
 *  QArchive Compressor Source
 * -----------------------------
 * Constructor and Destructor.
 * -----------------------------
*/
Compressor::Compressor(QObject *parent)
    : QObject(parent)
{
    return;
}

Compressor::Compressor(const QString& archive)
    : QObject(nullptr)
{
    setArchive(archive);
    return;
}

Compressor::Compressor(const QString& archive, const QStringList& files)
    : QObject(nullptr)
{
    setArchive(archive);
    addFiles(files);
    return;

}

Compressor::Compressor(const QString& archive, const QString& file)
    : QObject(nullptr)
{
    setArchive(archive);;
    addFiles(file);
    return;
}

Compressor::~Compressor()
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
 *  Compressor.
*/
void Compressor::setArchive(const QString& archive)
{
    if(mutex.tryLock()) {
        archivePath = QDir::cleanPath(archive);
        mutex.unlock();
    }
    return;
}

void Compressor::setArchiveFormat(short type)
{
    if(mutex.tryLock()) {
        archiveFormat = type;
        mutex.unlock();
    }
    return;
}

void Compressor::addFiles(const QString& file)
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

void Compressor::addFiles(const QStringList& files)
{
    if(mutex.tryLock()) {
        nodes << files;
        nodes.removeDuplicates();
        mutex.unlock();
    }
    return;
}

void Compressor::removeFiles(const QString& file)
{
    if(mutex.tryLock()) {
        if(!nodes.isEmpty()) {
            nodes.removeAll(file);
        }
        mutex.unlock();
    }
    return;
}

void Compressor::removeFiles(const QStringList& files)
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
 *  Compressor
*/
bool Compressor::isRunning() const
{
    return Promise->isRunning();
}

void Compressor::start(void)
{
    if(!mutex.tryLock()) {
        return;
    }
    Promise = new QFuture<void>;
    *Promise = QtConcurrent::run(this, &Compressor::startCompression);
    return;
}

void Compressor::stop(void)
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
 *  Compressor
*/
void Compressor::checkNodes()
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

void Compressor::getArchiveFormat()
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

void Compressor::startCompression()
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
 * Class Reader.
 * ------------------------
 *  QArchive Reader Source.
 * ---------------------------
 * Constructor and Destructor.
 * ---------------------------
*/
Reader::Reader(QObject *parent)
    : QObject(parent)
{
    return;
}
Reader::Reader(const QString& archive)
    : QObject(nullptr)
{
    setArchive(archive);
    return;
}

Reader::~Reader()
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
 *  Reader
*/
void Reader::setArchive(const QString& archive)
{
    if(mutex.tryLock()) {
        Archive = QDir::cleanPath(archive);
        mutex.unlock();
    }
    return;
}

const QStringList& Reader::listFiles()
{
    return Files;
}

void Reader::clear()
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
 *  Reader
*/
bool Reader::isRunning() const
{
    return Promise->isRunning();
}

void Reader::start(void)
{
    if(!mutex.tryLock()) {
        return;
    }
    Promise = new QFuture<void>;
    *Promise = QtConcurrent::run(this, &Reader::startReading);
    return;
}

void Reader::stop(void)
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
 *  Reader
*/
void Reader::startReading()
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
 *  Class Reader Ends Here.
 * ------------------------------------
*/

// =============================================================== //
