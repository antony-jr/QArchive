#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>

#include <qarchivediskextractor_p.hpp>
#include <qarchiveutils_p.hpp>
#include <qarchive_enums.hpp>

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

/*
 * Small patch to fix errors on MSC ,
 * fixed by https://github.com/hcaihao
*/
#if defined(_MSC_VER)
#include <BaseTsd.h>
#include <io.h>
typedef SSIZE_T ssize_t;
#endif

/*
 * Helpful macros to check if an archive error is caused due to
 * faulty passwords.
 * Expects a pointer to a struct archive , returns 1 if password
 * is needed or incorrect.
*/
#define PASSWORD_NEEDED(a) !strcmp(archive_error_string(a) , "Passphrase required for this entry")
#define PASSWORD_INCORRECT(a) !strcmp(archive_error_string(a) , "Incorrect passphrase")


using namespace QArchive;

/*
 * DiskExtractorPrivate constructor constructs the object which is the private class
 * implementation to the DiskExtractor.
 * This class is responsible for extraction and information retrival of the data
 * inside an archive.
 * This class only extracts the data to the disk and hence the name DiskExtractor.
 * This class will not be able to extract or work in-memory.
*/
DiskExtractorPrivate::DiskExtractorPrivate()
    : QObject(),
      n_Flags(ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_SECURE_NODOTDOT)
{
    m_Info.reset(new QJsonObject);
    m_ExtractFilters.reset(new QStringList);
}

DiskExtractorPrivate::~DiskExtractorPrivate()
{
}


/*
 * Sets the given pointer to QFile as the Archive file itself.
 */
void DiskExtractorPrivate::setArchive(QFile *archive)
{
    if(b_Started || b_Paused) {
        return;
    }
    clear();
    m_Archive = archive;
    return;
}

/*
 * Sets the archive path as the given QString which will be later
 * opened to be used as the Archive file.
*/
void DiskExtractorPrivate::setArchive(const QString &archivePath)
{
    if(b_Started || b_Paused || archivePath.isEmpty()) {
        return;
    }
    clear();
    m_ArchivePath = archivePath;
    return;
}

/* Blocksize to be used when extracting the given archive. */
void DiskExtractorPrivate::setBlockSize(int n)
{
    if(b_Started || b_Paused) {
        return;
    }
    n_BlockSize = n;
    return;
}

/* Sets the directory where the extraction data to be extracted.*/
void DiskExtractorPrivate::setOutputDirectory(const QString &destination)
{
    if(b_Started || b_Paused || destination.isEmpty()) {
        return;
    }
    m_OutputDirectory = destination + "/";
    return;
}

/* Enables/Disables the progress of the extraction with respect to the
 * given bool. */
void DiskExtractorPrivate::setCalculateProgress(bool c)
{
    b_NoProgress = !c;
    return;
}

/*
 * Sets the password for the archive when extracting the data. */
void DiskExtractorPrivate::setPassword(const QString &passwd)
{
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

/*
 * Adds extract filters , if set , only the files in the filter
 * will be extracted , the filter has to correspond to the exact
 * path given in the archive.
*/
void DiskExtractorPrivate::addFilter(const QString &filter)
{
    if(b_Started || b_Paused || filter.isEmpty()) {
        return;
    }
    *(m_ExtractFilters.data()) << filter;
    return;
}

/*
 * Overload of addFilter to accept list of QStrings. */
void DiskExtractorPrivate::addFilter(const QStringList &filters)
{
    if(b_Started || b_Paused || filters.isEmpty()) {
        return;
    }
    *(m_ExtractFilters.data()) << filters;
    return;
}

/*
 * Clears all internal data and sets it back to default. */
void DiskExtractorPrivate::clear()
{
    if(b_Started) {
        return;
    }
    n_BlockSize = 10240;
    n_PasswordTriedCountGetInfo = n_PasswordTriedCountExtract = 0;
    n_TotalEntries = -1;
    b_PauseRequested = b_CancelRequested = b_Paused = b_Started = b_Finished = b_ArchiveOpened = false;

    m_ArchivePath.clear();
#if ARCHIVE_VERSION_NUMBER >= 3003003
    m_Password.clear();
#endif
    m_OutputDirectory.clear();
    m_ArchiveRead.clear();
    m_ArchiveWrite.clear();
    m_Info.reset(new QJsonObject);
    m_ExtractFilters->clear();

    /* if m_Archive is allocated by DiskExtractor then  parent to child deallocation
     * should take care of the deallocation else if it is given by the user then it is
     * kept untouched and the ownership is dropped.
    */
    m_Archive = nullptr;
    return;
}

/*
 * Returns the information of the archive through info signal. */
void DiskExtractorPrivate::getInfo()
{
    if(b_Started || b_Paused) {
        return;
    } else if(!m_Info->isEmpty()) {
        emit info(*(m_Info.data()));
        return;
    }
    short errorCode = NoError;

    /* Open the Archive. */
    if((errorCode = openArchive()) != NoError) {
        emit error(errorCode , m_ArchivePath);
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
        emit error(errorCode , m_Archive->fileName());
    }
    return;
}

/*
 * Starts the extractor. */
void DiskExtractorPrivate::start()
{
    if(b_Started || b_Paused) {
        return;
    }
    short errorCode = NoError;

    /* Open the Archive. */
    if((errorCode = openArchive()) != NoError) {
        emit error(errorCode , m_ArchivePath);
        return;
    }

    /* Check and Set Output Directory. */
    if(!m_OutputDirectory.isEmpty()) {
        if((errorCode = checkOutputDirectory()) != NoError) {
            emit error(errorCode , m_Archive->fileName());
            return;
        }
    }

    /* Get basic information about the archive if the user wants progress on the
     * extraction. */
    if(n_TotalEntries == -1 && !b_NoProgress) {
        errorCode = getTotalEntriesCount();
        if(n_TotalEntries == -1) {
            /* If the total entries is unchanged then there must be an
            * error. */
#if ARCHIVE_VERSION_NUMBER >= 3003003 
  	    if(errorCode == ArchivePasswordIncorrect || errorCode == ArchivePasswordNeeded) {
                emit extractionRequirePassword(n_PasswordTriedCountExtract);
                ++n_PasswordTriedCountExtract;
            } else {
                emit error(errorCode , m_Archive->fileName());
                return;
            }
#else
	    emit error(errorCode , m_Archive->fileName());
	    return;
#endif
        }
    }


    /* All Okay then start the extraction. */
    b_Started = true;
    emit started();

    errorCode = extract();
    if(errorCode == NoError) {
        b_Started = false;
        b_Finished = true;
        emit finished();
    } 
#if ARCHIVE_VERSION_NUMBER >= 3003003
    else if(errorCode == ArchivePasswordIncorrect || errorCode == ArchivePasswordNeeded) {
        b_Started = false;
        emit extractionRequirePassword(n_PasswordTriedCountExtract);
        ++n_PasswordTriedCountExtract;
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
        emit error(errorCode , m_Archive->fileName());
    }
    return;
}

/*
 * Pauses the extractor. */
void DiskExtractorPrivate::pause()
{
    if(b_Started && !b_Paused) {
        b_PauseRequested = true;
    }
    return;
}

/*
 * Resumes the extractor. */
void DiskExtractorPrivate::resume()
{
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
        emit finished();
    } 
#if ARCHIVE_VERSION_NUMBER >= 3003003
    else if(ret == ArchivePasswordIncorrect || ret == ArchivePasswordNeeded) {
        b_Started = false;
        emit extractionRequirePassword(n_PasswordTriedCountExtract);
        ++n_PasswordTriedCountExtract;
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
        emit error(ret , m_Archive->fileName());
    }
    return;
}

/*
 * Cancels the extraction. */
void DiskExtractorPrivate::cancel()
{
    if(b_Started && !b_Paused && !b_Finished) {
        b_CancelRequested = true;
    }
    return;
}


short DiskExtractorPrivate::openArchive()
{
    if(m_ArchivePath.isEmpty() && m_Archive == nullptr) {
        return ArchiveNotGiven;
    } else if(b_ArchiveOpened) {
        return NoError;
    }
    /* Check and Open the given archive.
     *
     * Note:
     * At this point of code either m_ArchivePath or m_Archive has to be
     * set or else the function should have exited with an error signal.
    */
    if(!m_ArchivePath.isEmpty()) {

        try {
            m_Archive = new QFile(this);
        } catch ( ... ) {
            m_Archive = nullptr; /* Just a precaution. */
            return NotEnoughMemory;
        }

        QFileInfo info(m_ArchivePath);
        /* Check if the file exists. */
        if(!info.exists()) {
            m_Archive->deleteLater();
            m_Archive = nullptr;
            return ArchiveDoesNotExists;
        } else if(!info.isFile()) { /* Check if its really a file. */
            m_Archive->deleteLater();
            m_Archive = nullptr;
            return InvalidArchiveFile;
        }

        m_Archive->setFileName(m_ArchivePath);

        /* Check if we have the permission to read it. */
        auto perm = m_Archive->permissions();
        if(
            !(perm & QFileDevice::ReadUser) &&
            !(perm & QFileDevice::ReadGroup) &&
            !(perm & QFileDevice::ReadOther)
        ) {
            m_Archive->deleteLater();
            m_Archive = nullptr;
            return NoPermissionToReadArchive;
        }

        /* Finally open the file. */
        if(!m_Archive->open(QIODevice::ReadOnly)) {
            m_Archive->deleteLater();
            m_Archive = nullptr;
            return CannotOpenArchive;
        }
    } else {

        /* Check if exists */
        if(!m_Archive->exists()) {
            return ArchiveDoesNotExists;
        } else if(!m_Archive->isReadable()) { /* Check if it is readable. */
            return ArchiveIsNotReadable;
        } else if(!m_Archive->isOpen()) { /* Check if it is opened. */
            return ArchiveIsNotOpened;
        }
    }
    b_ArchiveOpened = true;
    return NoError;
}

short DiskExtractorPrivate::checkOutputDirectory()
{
    QFileInfo info(m_OutputDirectory + "/");
    /* Check if its a directory and not a file , Also check if it exists. */
    if(!info.exists() || !info.isDir()) {
        return InvalidOutputDirectory;
    }

    /* Check if we have the permission to read and write. */
    if(!info.isWritable() || !info.isReadable()) {
        return NoPermissionToWrite;
    }
    m_OutputDirectory = info.absoluteFilePath();
    return NoError;
}

short DiskExtractorPrivate::extract()
{
    if(m_Archive == nullptr) {
        return ArchiveNotGiven;
    }
    int ret = 0;
    short err = NoError;
    archive_entry *entry = nullptr;

    if(m_ArchiveRead.isNull() && m_ArchiveWrite.isNull()) {
        n_ProcessedEntries = 0;
        auto prev = m_Archive->pos();
        m_Archive->seek(0);

        m_ArchiveRead = QSharedPointer<struct archive>(archive_read_new(), ArchiveReadDestructor);
        m_ArchiveWrite = QSharedPointer<struct archive>(archive_write_disk_new(), ArchiveWriteDestructor);
        if(!m_ArchiveRead.data() || !m_ArchiveWrite.data()) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            m_Archive->seek(prev);
            return NotEnoughMemory;
        }

#if ARCHIVE_VERSION_NUMBER >= 3003003 
	if(!m_Password.isEmpty()) {
            archive_read_add_passphrase(m_ArchiveRead.data(), m_Password.toLatin1().constData());
        }
#endif
	archive_read_support_format_all(m_ArchiveRead.data());
        archive_read_support_filter_all(m_ArchiveRead.data());
        if((ret = archive_read_open_fd(m_ArchiveRead.data(), m_Archive->handle(),n_BlockSize))) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            m_Archive->seek(prev);
            return ArchiveReadError;
        }

        if((ret = archive_write_disk_set_options(m_ArchiveWrite.data(), n_Flags))) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            m_Archive->seek(prev);
            return ArchiveWriteError;
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
                m_Archive->seek(prev);
                return err;
            }

            if((err = writeData(entry))) {
                m_ArchiveRead.clear();
                m_ArchiveWrite.clear();
                m_Archive->seek(prev);
                return err;
            }

            if(n_TotalEntries > 0) {
                ++n_ProcessedEntries;
                emit progress(QString(archive_entry_pathname(entry)),
                              n_ProcessedEntries,
                              n_TotalEntries,
                              (n_ProcessedEntries*100)/n_TotalEntries);
            } else {
                emit progress(QString(archive_entry_pathname(entry)), 0, 0, 0);
            }

            QCoreApplication::processEvents();
            if(b_PauseRequested) {
                b_PauseRequested = false;
                return OperationPaused;
            }

            if(b_CancelRequested) {
                b_CancelRequested = false;
                m_ArchiveRead.clear();
                m_ArchiveWrite.clear();
                m_Archive->seek(prev);
                return OperationCanceled;
            }
        }
        m_Archive->seek(prev);
    } else {
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
                m_Archive->seek(0);
                return err;
            }

            if((err = writeData(entry))) {
                m_ArchiveRead.clear();
                m_ArchiveWrite.clear();
                m_Archive->seek(0);
                return err;
            }

            if(n_TotalEntries > 0) {
                ++n_ProcessedEntries;
                emit progress(QString(archive_entry_pathname(entry)),
                              n_ProcessedEntries,
                              n_TotalEntries,
                              (n_ProcessedEntries*100)/n_TotalEntries);
            } else {
                emit progress(QString(archive_entry_pathname(entry)), 0, 0, 0);
            }

            /*
             * Process events to know if the user canceled
             * or paused the extraction
            */
            QCoreApplication::processEvents();
            if(b_PauseRequested) {
                b_PauseRequested = false;
                return OperationPaused;
            }

            if(b_CancelRequested) {
                b_CancelRequested = false;
                m_ArchiveRead.clear();
                m_ArchiveWrite.clear();
                m_Archive->seek(0);
                return OperationCanceled;
            }

        }
    }

    /* free memory. */
    m_ArchiveRead.clear();
    m_ArchiveWrite.clear();
    return NoError;
}

short DiskExtractorPrivate::writeData(struct archive_entry *entry)
{
    if(m_ArchiveRead.isNull() || m_ArchiveWrite.isNull() || m_Archive == nullptr) {
        return ArchiveNotGiven;
    }

    if(!m_ExtractFilters->isEmpty() &&
       !m_ExtractFilters->contains(QString(archive_entry_pathname(entry)))) {
        return NoError;
    }

    if(!m_OutputDirectory.isEmpty()) {
        char *new_entry = concat(m_OutputDirectory.toLatin1().constData(), archive_entry_pathname(entry));
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

short DiskExtractorPrivate::getTotalEntriesCount()
{
    if(m_Archive == nullptr) {
        return ArchiveNotGiven;
    }

    auto prev = m_Archive->pos();
    m_Archive->seek(0);

    int ret = 0;
    int count = 0;
    archive_entry *entry = nullptr;
    struct archive *inArchive = archive_read_new();
    if(!inArchive) {
        m_Archive->seek(prev);
        return NotEnoughMemory;
    }

#if ARCHIVE_VERSION_NUMBER >= 3003003 
    if(!m_Password.isEmpty()) {
        archive_read_add_passphrase(inArchive, m_Password.toLatin1().constData());
    }
#endif

    archive_read_support_format_all(inArchive);
    archive_read_support_filter_all(inArchive);
    if((ret = archive_read_open_fd(inArchive, m_Archive->handle(),n_BlockSize))) {
        archive_read_close(inArchive);
        archive_read_free(inArchive);
        m_Archive->seek(prev);
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
            m_Archive->seek(prev);
            return err;
        }
        count += 1;
    }

    m_Archive->seek(prev); /* seek to the last position. */
    /* set total number of entries. */
    n_TotalEntries = count;

    /* free memory. */
    archive_read_close(inArchive);
    archive_read_free(inArchive);
    return NoError;
}

short DiskExtractorPrivate::processArchiveInformation()
{
    if(m_Archive == nullptr) {
        return ArchiveNotGiven;
    }

    auto prev = m_Archive->pos();
    m_Archive->seek(0);

    int ret = 0;
    archive_entry *entry = nullptr;
    struct archive *inArchive = archive_read_new();
    if(!inArchive) {
        m_Archive->seek(prev);
        return NotEnoughMemory;
    }
#if ARCHIVE_VERSION_NUMBER >= 3003003 
    if(!m_Password.isEmpty()) {
        archive_read_add_passphrase(inArchive, m_Password.toLatin1().constData());
    }
#endif 
    archive_read_support_format_all(inArchive);
    archive_read_support_filter_all(inArchive);
    if((ret = archive_read_open_fd(inArchive, m_Archive->handle(),n_BlockSize))) {
        archive_read_close(inArchive);
        archive_read_free(inArchive);
        m_Archive->seek(prev);
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
            m_Archive->seek(prev);
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
            CurrentEntry.insert("LastAccessedTime", QJsonValue((QDateTime::fromTime_t(lastAccessT)).toString(Qt::ISODate)));
        } else {
            CurrentEntry.insert("LastAccessedTime", "Unknown");
        }

        if(lastModT) {
            CurrentEntry.insert("LastModifiedTime", QJsonValue((QDateTime::fromTime_t(lastModT)).toString(Qt::ISODate)));
        } else {
            CurrentEntry.insert("LastModifiedTime", "Unknown");
        }

        if(lastStatusModT) {
            CurrentEntry.insert("LastStatusModifiedTime", QJsonValue((QDateTime::fromTime_t(lastStatusModT)).toString(Qt::ISODate)));
        } else {
            CurrentEntry.insert("LastStatusModifiedTime", "Unknown");
        }
        m_Info->insert(CurrentFile, CurrentEntry);
    }

    m_Archive->seek(prev);

    /* set total number of entries. */
    n_TotalEntries = m_Info->size();

    /* free memory. */
    archive_read_close(inArchive);
    archive_read_free(inArchive);
    return NoError;
}

QString DiskExtractorPrivate::errorCodeToString(short errorCode){
	QString errorString;
	switch(errorCode){
		case ArchiveNotGiven:
			errorString = QString("QArchive::ArchiveNotGiven");
			break;
		case ArchiveDoesNotExists:
			errorString = QString("QArchive::ArchiveDoesNotExists");
			break;
		case ArchiveReadError:
			errorString = QString("QArchive::ArchiveReadError");
			break;
    		case ArchiveWriteError:
			errorString = QString("QArchive::ArchiveWriteError");
			break;
		case ArchiveHeaderWriteError:
			errorString = QString("QArchive::ArchiveHeaderWriteError");
			break;
		case ArchiveCorrupted:
			errorString = QString("QArchive::ArchiveCorrupted");
			break;
		case ArchiveIsNotReadable:
			errorString = QString("QArchive::ArchiveIsNotReadable");
			break;
		case ArchiveIsNotOpened:
			errorString = QString("QArchive::ArchiveIsNotOpened");
			break;
		case ArchivePasswordNeeded:
			errorString = QString("QArchive::ArchivePasswordNeeded");
			break;
		case ArchivePasswordIncorrect:
			errorString = QString("QArchive::ArchivePasswordIncorrect");
			break;
		case CannotOpenArchive:
			errorString = QString("QArchive::CannotOpenArchive");
			break;
		case NoPermissionToReadArchive:
			errorString = QString("QArchive::NoPermissionToReadArchive");
			break;
		case InvalidOutputDirectory:
			errorString = QString("QArchive::InvalidOutputDirectory");
			break;
		case InvalidArchiveFile:
			errorString = QString("QArchive::InvalidArchiveFile");
			break;
		default:
			break;
			
	}
	return errorString;
}
