#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>

#include <qarchivediskextractor_p.hpp>
#include <qarchiveutils_p.hpp>
#include <qarchive_enums.hpp>
#include <qarchiveioreader_p.hpp>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
}

#if defined(__APPLE__)
#define st_atim st_atimespec.tv_sec
#define st_ctim st_ctimespec.tv_sec
#define st_mtim st_mtimespec.tv_sec
#elif defined(_WIN32) && !defined(__CYGWIN__)
#define st_atim st_atime
#define st_ctim st_ctime
#define st_mtim st_mtime
#else
#define st_atim st_atim.tv_sec
#define st_ctim st_ctim.tv_sec
#define st_mtim st_mtim.tv_sec
#endif

// Helpful macros to check if an archive error is caused due to
// faulty passwords.
// Expects a pointer to a struct archive , returns 1 if password
// is needed or incorrect.
#define PASSWORD_NEEDED(a) !qstrcmp(archive_error_string(a) ,"Passphrase required for this entry")
#define PASSWORD_INCORRECT(a) !qstrcmp(archive_error_string(a) , "Incorrect passphrase")


// Private data structure which holds a QIODevice along with its buffer size
// to read at a time.
// This structure will be used as the client data for the archive callbacks.
struct ClientData_t {
    char *storage = nullptr;
    QArchive::IOReaderPrivate *io = nullptr;
};


// This callback will be called on archive open.
// This callback is simply used to avoid segmentation fault when
// the programmer mistakenly gives a QIODevice that has not opened.
static int archive_open_cb(struct archive *archive, void *data) {
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    if(!p) {
        // We surely need the reader handle to continue
        // any further.
        return ARCHIVE_FATAL;
    }
    if(!p->io->isOpen() ||
            !p->io->isReadable() ||
            !(p->storage) ||
            p->io->isSequential()) {
        return ARCHIVE_FATAL;
    }
    return ARCHIVE_OK;
}

static int archive_close_cb(struct archive *archive, void *data) {
    // Should not do anything to QIODevice pointer since
    // its a private object inside our class.
    // It will be managed automatically later.
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    if(p->storage) { // free any data that has been allocated.
        free(p->storage);
    }
    delete (p->io); //  Delete IOReaderPrivate.
    free(p); // free the client data allocated on creation.
    return ARCHIVE_OK;
}


// This read callback is called whenever libarchive needs 
// more data to crunch , this is very important since we have 
// to read the data from QIODevice.
static la_ssize_t archive_read_cb(struct archive *archive, void *data, const void **buffer) {
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    *buffer = (void*)p->storage;
    return p->io->read(p->storage);
}

// This is the most important callback function required for libarchive to work with
// QIODevice , this can't be set with the provided libarchive public functions.
// We will be using a private function to set this callback.
// This callback seeks the QIODevice with respect to whence which is the same as
// given in fseek and lseek.
//
// Without this function you cannot extract 7zip archives.
static int64_t archive_seek_cb(struct archive *archive, void *data, int64_t request, int whence) {
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    return static_cast<int64_t>(p->io->seek(request, whence));
}

// This is a custom functions which sets up the callbacks and other
// stuff for a libarchive struct.
static int archive_read_open_QIODevice(struct archive *archive, int blocksize, QIODevice *device) {
    // This client data will be freed on close ,
    // we don't need to worry about this.
    ClientData_t *p = (ClientData_t*)calloc(1, sizeof *p);
    p->io = new QArchive::IOReaderPrivate;
    p->io->setIODevice(device);
    p->io->setBlockSize(blocksize);
    p->storage = (char*)calloc(1, (blocksize < 1024) ?
                               sizeof(*(p->storage)) * 10204 :
                               sizeof(*(p->storage)) * blocksize);
    archive_read_set_open_callback(archive, archive_open_cb);
    archive_read_set_read_callback(archive, archive_read_cb);
    archive_read_set_seek_callback(archive, archive_seek_cb);
    archive_read_set_close_callback(archive, archive_close_cb);
    archive_read_set_callback_data(archive, (void*)p);
    return archive_read_open1(archive);
}

using namespace QArchive;

// DiskExtractorPrivate constructor constructs the object which is the private class
// implementation to the DiskExtractor.
// This class is responsible for extraction and information retrival of the data
// inside an archive.
// This class only extracts the data to the disk and hence the name DiskExtractor.
// This class will not be able to extract or work in-memory.
DiskExtractorPrivate::DiskExtractorPrivate()
    : QObject(),
      n_Flags(ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_SECURE_NODOTDOT) {
    m_Info.reset(new QJsonObject);
    m_ExtractFilters.reset(new QStringList);
}

DiskExtractorPrivate::~DiskExtractorPrivate() {
	clear();
}


// Sets the given pointer to QIODevice as the Archive file itself.
void DiskExtractorPrivate::setArchive(QIODevice *archive) {
    if(b_Started || b_Paused) {
        return;
    }
    clear();
    m_Archive = archive;
    return;
}

// Sets the archive path as the given QString which will be later
// opened to be used as the Archive file.
void DiskExtractorPrivate::setArchive(const QString &archivePath) {
    if(b_Started || b_Paused || archivePath.isEmpty()) {
        return;
    }
    clear();
    m_ArchivePath = archivePath;
    return;
}

// Blocksize to be used when extracting the given archive.
void DiskExtractorPrivate::setBlockSize(int n) {
    if(b_Started || b_Paused) {
        return;
    }
    n_BlockSize = n;
    return;
}

// Sets the directory where the extraction data to be extracted.
void DiskExtractorPrivate::setOutputDirectory(const QString &destination) {
    if(b_Started || b_Paused || destination.isEmpty()) {
        return;
    }
    m_OutputDirectory = destination + "/";
    return;
}

// Enables/Disables the progress of the extraction with respect to the
// given bool. 
void DiskExtractorPrivate::setCalculateProgress(bool c) {
    b_NoProgress = !c;
    return;
}

// Sets the password for the archive when extracting the data.
// This method should be accessible even if the extractor is started
// since the user may set password anytime. 
void DiskExtractorPrivate::setPassword(const QString &passwd) {
#if ARCHIVE_VERSION_NUMBER >= 3003003
    if(passwd.isEmpty()) {
        return;
    }
    m_Password = passwd;
#else
    (void)passwd;
#endif
    return;
}

// Adds extract filters , if set , only the files in the filter
// will be extracted , the filter has to correspond to the exact
// path given in the archive.
void DiskExtractorPrivate::addFilter(const QString &filter) {
    if(b_Started || b_Paused || filter.isEmpty()) {
        return;
    }
    *(m_ExtractFilters.data()) << filter;
    return;
}

// Overload of addFilter to accept list of QStrings.
void DiskExtractorPrivate::addFilter(const QStringList &filters) {
    if(b_Started || b_Paused || filters.isEmpty()) {
        return;
    }
    *(m_ExtractFilters.data()) << filters;
    return;
}

// Clears all internal data and sets it back to default.
void DiskExtractorPrivate::clear() {
    if(b_Started) {
        return;
    }
    n_BlockSize = 10240;
    n_PasswordTriedCountGetInfo = n_PasswordTriedCountExtract = 0;
    n_TotalEntries = -1;
    b_PauseRequested = b_CancelRequested = b_Paused = b_Started = b_Finished = b_ArchiveOpened = false;

    // TODO: do we need to reset n_BytesTotal here?
    n_BytesProcessed = 0;
    n_BytesTotal = 0;

    m_ArchivePath.clear();
#if ARCHIVE_VERSION_NUMBER >= 3003003
    m_Password.clear();
#endif
    m_OutputDirectory.clear();
    m_ArchiveRead.clear();
    m_ArchiveWrite.clear();
    m_Info.reset(new QJsonObject);
    m_ExtractFilters->clear();

    if(b_QIODeviceOwned){
	    m_Archive->close();
	    m_Archive->deleteLater();
    }else{
    	m_Archive = nullptr;
    }
    b_QIODeviceOwned = false;
    return;
}

// Returns the information of the archive through info signal.
void DiskExtractorPrivate::getInfo() {
    if(!m_Info->isEmpty()) {
        emit info(*(m_Info.data()));
        return;
    }
    short errorCode = NoError;

    // Open the Archive.
    if((errorCode = openArchive()) != NoError) {
        emit error(errorCode);
        return;
    }

    errorCode = processArchiveInformation();
    if(!errorCode) {
        emit info(*(m_Info.data()));
    }
#if ARCHIVE_VERSION_NUMBER >= 3003003
    else if(errorCode == ArchivePasswordIncorrect || errorCode == ArchivePasswordNeeded) {
        emit getInfoRequirePassword(n_PasswordTriedCountGetInfo);
        ++n_PasswordTriedCountGetInfo;
    }
#endif
    else {
        emit error(errorCode);
    }
    return;
}

void DiskExtractorPrivate::start() {
    if(b_Started || b_Paused) {
        return;
    }
    short errorCode = NoError;

    // Open the Archive.
    if((errorCode = openArchive()) != NoError) {
        emit error(errorCode);
        return;
    }

    // Check and Set Output Directory.
    if(!m_OutputDirectory.isEmpty()) {
        if((errorCode = checkOutputDirectory()) != NoError) {
            emit error(errorCode );
            return;
        }
    }


    // All Okay then start the extraction.
    b_Started = true;
    emit started();

    // Get basic information about the archive if the user wants progress on the
    // extraction. 
    if(n_TotalEntries == -1 && !b_NoProgress) {
        errorCode = getTotalEntriesCount();
        if(n_TotalEntries == -1) {
            // If the total entries is unchanged then there must be an
            // error.
#if ARCHIVE_VERSION_NUMBER >= 3003003
            b_Started = false;
            if(errorCode == ArchivePasswordIncorrect || errorCode == ArchivePasswordNeeded) {
                emit extractionRequirePassword(n_PasswordTriedCountExtract);
                ++n_PasswordTriedCountExtract;
            }
#endif
            emit error(errorCode );
            return;
        }
    }

    n_BytesProcessed = 0;

    errorCode = extract();
    if(errorCode == NoError) {
        b_Started = false;
        b_Finished = true;
        m_Archive->close();
	emit finished();
    }
#if ARCHIVE_VERSION_NUMBER >= 3003003
    else if(errorCode == ArchivePasswordIncorrect || errorCode == ArchivePasswordNeeded) {
        b_Started = false;
        emit extractionRequirePassword(n_PasswordTriedCountExtract);
        ++n_PasswordTriedCountExtract;
        emit error(errorCode );
    }
#endif
    else if(errorCode == OperationCanceled) {
        b_Started = false;
        emit canceled();
    } else if(errorCode == OperationPaused) {
        b_Started = false;
        b_Paused = true;
        emit paused();
    } else {
        b_Started = false;
        emit error(errorCode );
    }
    return;
}
 
// Pauses the extractor.
void DiskExtractorPrivate::pause() {
    if(b_Started && !b_Paused) {
        b_PauseRequested = true;
    }
    return;
}

// Resumes the extractor.
void DiskExtractorPrivate::resume() {
    if(!b_Paused) {
        return;
    }
    b_Paused = false;
    b_Started = true;
    emit resumed();

    short ret = extract();
    if(ret == NoError) {
        b_Started = false;
        b_Finished = true;
        m_Archive->close();
        emit finished();
    }
#if ARCHIVE_VERSION_NUMBER >= 3003003
    else if(ret == ArchivePasswordIncorrect || ret == ArchivePasswordNeeded) {
        b_Started = false;
        emit extractionRequirePassword(n_PasswordTriedCountExtract);
        ++n_PasswordTriedCountExtract;
        emit error(ret );
    }
#endif
    else if(ret == OperationCanceled) {
        b_Started = false;
        emit canceled();
    } else if(ret == OperationPaused) {
        b_Started = false;
        b_Paused = true;
        emit paused();
    } else {
        b_Started = false;
        emit error(ret );
    }
    return;
}

// Cancels the extraction.
void DiskExtractorPrivate::cancel() {
    if(b_Started && !b_Paused && !b_Finished) {
        b_CancelRequested = true;
    }
    return;
}


short DiskExtractorPrivate::openArchive() {
    if(m_ArchivePath.isEmpty() && !m_Archive) {
        return ArchiveNotGiven;
    } else if(b_ArchiveOpened) {
        return NoError;
    }
    // Check and Open the given archive.
    //
    // Note:
    // At this point of code either m_ArchivePath or m_Archive has to be
    // set or else the function should have exited with an error signal.
    if(!m_ArchivePath.isEmpty()) {

        QFileInfo info(m_ArchivePath);
        // Check if the file exists.
        if(!info.exists()) {
            return ArchiveDoesNotExists;
        } else if(!info.isFile()) { // Check if its really a file.
            return InvalidArchiveFile;
        }

        // Check if we have the permission to read it.
        auto perm = info.permissions();
        if(
            !(perm & QFileDevice::ReadUser) &&
            !(perm & QFileDevice::ReadGroup) &&
            !(perm & QFileDevice::ReadOther)
        ) {
            return NoPermissionToReadArchive;
        }


        QFile *file;
        try {
            file = new QFile(this);
	} catch ( ... ) {
            m_Archive = nullptr; // Just a precaution.
            return NotEnoughMemory;
        }
        file->setFileName(m_ArchivePath);

        // Finally open the file.
        if(!file->open(QIODevice::ReadOnly)) {
            file->deleteLater();
            return CannotOpenArchive;
        }

	b_QIODeviceOwned = true;
        m_Archive = (QIODevice*)file;
    } else {

        if(!m_Archive->isOpen()) { // Check if it is opened.
            return ArchiveIsNotOpened;
        } else if(!m_Archive->isReadable()) { // Check if it is readable. 
            return ArchiveIsNotReadable;
        }
    }
    b_ArchiveOpened = true;
    return NoError;
}

short DiskExtractorPrivate::checkOutputDirectory() {
    QFileInfo info(m_OutputDirectory + "/");
    // Check if its a directory and not a file , Also check if it exists. 
    if(!info.exists() || !info.isDir()) {
        return InvalidOutputDirectory;
    }

    // Check if we have the permission to read and write.
    if(!info.isWritable() || !info.isReadable()) {
        return NoPermissionToWrite;
    }
    m_OutputDirectory = info.absoluteFilePath();
    return NoError;
}

short DiskExtractorPrivate::extract() {
    if(m_Archive == nullptr) {
        return ArchiveNotGiven;
    }
    int ret = 0;
    short err = NoError;
    archive_entry *entry = nullptr;

    if(m_ArchiveRead.isNull() && m_ArchiveWrite.isNull()) {
        n_ProcessedEntries = 0;

        m_ArchiveRead = QSharedPointer<struct archive>(archive_read_new(), ArchiveReadDestructor);
        m_ArchiveWrite = QSharedPointer<struct archive>(archive_write_disk_new(), ArchiveWriteDestructor);
        if(!m_ArchiveRead.data() || !m_ArchiveWrite.data()) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            return NotEnoughMemory;
        }

#if ARCHIVE_VERSION_NUMBER >= 3003003
        if(!m_Password.isEmpty()) {
            archive_read_add_passphrase(m_ArchiveRead.data(), m_Password.toUtf8().constData());
        }
#endif
        archive_read_support_format_all(m_ArchiveRead.data());
        archive_read_support_filter_all(m_ArchiveRead.data());

        if((ret = archive_read_open_QIODevice(m_ArchiveRead.data(), n_BlockSize, m_Archive))) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            return ArchiveReadError;
        }

        if((ret = archive_write_disk_set_options(m_ArchiveWrite.data(), n_Flags))) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            return ArchiveWriteError;
        }

    }
    for (;;) {
            ret = archive_read_next_header(m_ArchiveRead.data(), &entry);
            if (ret == ARCHIVE_EOF) {
                break;
            }
            if (ret != ARCHIVE_OK) {
                err = ArchiveCorrupted;
                if(PASSWORD_NEEDED(m_ArchiveRead.data())) {
                    err = ArchivePasswordNeeded;
                } else if(PASSWORD_INCORRECT(m_ArchiveRead.data())) {
                    err = ArchivePasswordIncorrect;
                }
                m_ArchiveRead.clear();
                m_ArchiveWrite.clear();
                return err;
            }

	    err = writeData(entry);
	    if(err == OperationPaused){
		    return err;
	    }else if(err){ // NoError = 0
		    m_ArchiveRead.clear();
		    m_ArchiveWrite.clear(); 
		    return err;
	    }
	    ++n_ProcessedEntries;
	
	    // Report final progress signal after extracting the file fully.
	    if(n_BytesTotal > 0 && n_TotalEntries > 0){
	    		emit progress(QString(archive_entry_pathname(entry)),
                              n_ProcessedEntries,
                              n_TotalEntries,
                              n_BytesProcessed, n_BytesTotal);
	     }else{
			emit progress(QString(archive_entry_pathname(entry)),
			    1,
			    1,
			    1,
			    1);

	     }
	     QCoreApplication::processEvents(); // call event loop for the signal to take effect.
    }

    /* free memory. */
    m_ArchiveRead.clear();
    m_ArchiveWrite.clear(); 
    return NoError;
}

short DiskExtractorPrivate::writeData(struct archive_entry *entry) {
    if(m_ArchiveRead.isNull() || m_ArchiveWrite.isNull() || m_Archive == nullptr) {
        return ArchiveNotGiven;
    }

    if(!m_ExtractFilters->isEmpty() &&
            !m_ExtractFilters->contains(QString(archive_entry_pathname(entry)))) {
        return NoError;
    }

    if(!m_OutputDirectory.isEmpty()) {
        char *new_entry = concat(m_OutputDirectory.toUtf8().constData(), archive_entry_pathname(entry));
        archive_entry_set_pathname(entry, new_entry);
        free(new_entry);
    }

    int ret = archive_write_header(m_ArchiveWrite.data(), entry);
    if (ret == ARCHIVE_OK) {
        const void *buff;
        size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
        int64_t offset;
#else
        off_t offset;
#endif
        for (;;) {
            ret = archive_read_data_block(m_ArchiveRead.data(), &buff, &size, &offset);
            if (ret == ARCHIVE_EOF) {
                break;
            } else if (ret != ARCHIVE_OK) {
                short err = ArchiveCorrupted;
                if(PASSWORD_NEEDED(m_ArchiveRead.data())) {
                    err = ArchivePasswordNeeded;
                } else if(PASSWORD_INCORRECT(m_ArchiveRead.data())) {
                    err = ArchivePasswordIncorrect;
                }
                return err;
            } else {
                ret = archive_write_data_block(m_ArchiveWrite.data(), buff, size, offset);
                if (ret != ARCHIVE_OK) {
                    return ArchiveWriteError;
                }
                n_BytesProcessed += size;
		if(n_BytesTotal > 0 && n_TotalEntries > 0){
	    		emit progress(QString(archive_entry_pathname(entry)),
                              n_ProcessedEntries,
                              n_TotalEntries,
                              n_BytesProcessed, n_BytesTotal);
		}

	    }

	    // Allow the execution of the event loop
            QCoreApplication::processEvents();

	    // Check for pause and cancel requests.
	    if(b_PauseRequested) {
                b_PauseRequested = false;
                return OperationPaused;
            }else if(b_CancelRequested) {
                b_CancelRequested = false;
                return OperationCanceled;
            }

        }
    } else {
        return ArchiveHeaderWriteError;
    }

    ret = archive_write_finish_entry(m_ArchiveWrite.data());
    if (ret == ARCHIVE_FATAL) {
        return ArchiveHeaderWriteError;
    }
    return NoError;
}

short DiskExtractorPrivate::getTotalEntriesCount() {
    if(!m_Archive) {
        return ArchiveNotGiven;
    }

    n_BytesTotal = 0;
    int ret = 0;
    int count = 0;
    archive_entry *entry = nullptr;
    struct archive *inArchive = archive_read_new();
    if(!inArchive) {
        return NotEnoughMemory;
    }

#if ARCHIVE_VERSION_NUMBER >= 3003003
    if(!m_Password.isEmpty()) {
        archive_read_add_passphrase(inArchive, m_Password.toUtf8().constData());
    }
#endif

    archive_read_support_format_all(inArchive);
    archive_read_support_filter_all(inArchive);
    if((ret = archive_read_open_QIODevice(inArchive, n_BlockSize, m_Archive))) {
        archive_read_close(inArchive);
        archive_read_free(inArchive);
        return ArchiveReadError;
    }
    for (;;) {
        ret = archive_read_next_header(inArchive, &entry);
        if (ret == ARCHIVE_EOF) {
            break;
        }
        if (ret != ARCHIVE_OK) {
            short err = ArchiveCorrupted;
            if(PASSWORD_NEEDED(inArchive)) {
                err = ArchivePasswordNeeded;
            } else if(PASSWORD_INCORRECT(inArchive)) {
                err = ArchivePasswordIncorrect;
            }
            archive_read_close(inArchive);
            archive_read_free(inArchive);
            return err;
        }
        count += 1;
        n_BytesTotal += archive_entry_size(entry);
        QCoreApplication::processEvents();
    }

    // set total number of entries.
    n_TotalEntries = count;

    // free memory.
    archive_read_close(inArchive);
    archive_read_free(inArchive);
    return NoError;
}

short DiskExtractorPrivate::processArchiveInformation() {
    if(m_Archive == nullptr) {
        return ArchiveNotGiven;
    }

    int ret = 0;
    archive_entry *entry = nullptr;
    struct archive *inArchive = archive_read_new();
    if(!inArchive) {
        return NotEnoughMemory;
    }
#if ARCHIVE_VERSION_NUMBER >= 3003003
    if(!m_Password.isEmpty()) {
        archive_read_add_passphrase(inArchive, m_Password.toUtf8().constData());
    }
#endif
    archive_read_support_format_all(inArchive);
    archive_read_support_filter_all(inArchive);

    if((ret = archive_read_open_QIODevice(inArchive, n_BlockSize, m_Archive))) {
        archive_read_close(inArchive);
        archive_read_free(inArchive);
        return ArchiveReadError;
    }
    for (;;) {
        ret = archive_read_next_header(inArchive, &entry);
        if (ret == ARCHIVE_EOF) {
            break;
        }
        if (ret != ARCHIVE_OK) {
            short err = ArchiveCorrupted;
            if(PASSWORD_NEEDED(inArchive)) {
                err = ArchivePasswordNeeded;
            } else if(PASSWORD_INCORRECT(inArchive)) {
                err = ArchivePasswordIncorrect;
            }
            archive_read_close(inArchive);
            archive_read_free(inArchive);
            return err;
        }
        QString CurrentFile = QString(archive_entry_pathname(entry));
        QJsonObject CurrentEntry;
        auto entry_stat = archive_entry_stat(entry);
        qint64 size = (qint64)entry_stat->st_size;
        QString sizeUnits = "Bytes";
        if(size == 0) {
            sizeUnits = "None";
            size = 0;
        } else if(size < 1024) {
            sizeUnits = "Bytes";
            size = size;
        } else if(size >= 1024 && size < 1048576) {
            sizeUnits = "KiB";
            size /= 1024;
        } else if(size >= 1048576 && size < 1073741824) {
            sizeUnits = "MiB";
            size /= 1048576;
        } else {
            sizeUnits = "GiB";
            size /= 1073741824;
        }

        // MSVC (and maybe Windows in general?) workaround
#if defined(_WIN32) && !defined(__CYGWIN__)
        qint64 blockSizeInBytes = 512;
        qint64 blocks = (qint64) (entry_stat->st_size / blockSizeInBytes);
#else
        qint64 blockSizeInBytes = (qint64)entry_stat->st_blksize;
        qint64 blocks = (qint64)entry_stat->st_blocks;
#endif
        auto lastAccessT = entry_stat->st_atim;
        auto lastModT = entry_stat->st_mtim;
        auto lastStatusModT = entry_stat->st_ctim;

        QFileInfo fileInfo(CurrentFile);

        auto ft = archive_entry_filetype(entry);
        QString FileType;
        switch(ft) {
        case AE_IFREG: // Regular file
            FileType = "RegularFile";
            break;
        case AE_IFLNK: // Link
            FileType = "SymbolicLink";
            break;
        case AE_IFSOCK: // Socket
            FileType = "Socket";
            break;
        case AE_IFCHR: // Character Device
            FileType = "CharacterDevice";
            break;
        case AE_IFBLK: // Block Device
            FileType = "BlockDevice";
            break;
        case AE_IFDIR: // Directory.
            FileType = "Directory";
            break;
        case AE_IFIFO: // Named PIPE. (fifo)
            FileType = "NamedPipe";
            break;
        default:
            FileType = "UnknownFile";
            break;
        };
        // Set the values.
        if(FileType != "RegularFile") {
            CurrentEntry.insert("FileName", getDirectoryFileName(CurrentFile));
        } else {
            CurrentEntry.insert("FileName", fileInfo.fileName());
        }

        CurrentEntry.insert("FileType", QJsonValue(FileType));
        CurrentEntry.insert("Size", QJsonValue(size));
        CurrentEntry.insert("SizeUnit", sizeUnits);
        CurrentEntry.insert("BlockSize", QJsonValue(blockSizeInBytes));
        CurrentEntry.insert("BlockSizeUnit", "Bytes");
        CurrentEntry.insert("Blocks", QJsonValue(blocks));
        if(lastAccessT) {
            CurrentEntry.insert("LastAccessedTime",
			    QJsonValue(
				   (QDateTime::fromTime_t(lastAccessT)).toString(Qt::ISODate)));
        } else {
            CurrentEntry.insert("LastAccessedTime", "Unknown");
        }

        if(lastModT) {
            CurrentEntry.insert("LastModifiedTime",
			    QJsonValue((QDateTime::fromTime_t(lastModT)).toString(Qt::ISODate)));
        } else {
            CurrentEntry.insert("LastModifiedTime", "Unknown");
        }

        if(lastStatusModT) {
            CurrentEntry.insert("LastStatusModifiedTime",
		QJsonValue((QDateTime::fromTime_t(lastStatusModT)).toString(Qt::ISODate)));
        } else {
            CurrentEntry.insert("LastStatusModifiedTime", "Unknown");
        }
        m_Info->insert(CurrentFile, CurrentEntry);
        n_BytesTotal += archive_entry_size(entry);
	QCoreApplication::processEvents();
    }

    // set total number of entries.
    n_TotalEntries = m_Info->size();

    // free memory.
    archive_read_close(inArchive);
    archive_read_free(inArchive);
    return NoError;
}
