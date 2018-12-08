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

#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>

#include <qarchivediskextractor_p.hpp>
#include <qarchive_enums.hpp>

using namespace QArchive;

#define DISK_EXTRACTOR_CONSTRUCTOR QObject(),\
				   _nFlags(ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_SECURE_NODOTDOT),\
				   _mInfo(QSharedPointer<QJsonObject>(new QJsonObject)), \
				   _mExtractFilters(QSharedPointer<QStringList>(new QStringList))

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

#define PASSWORD_NEEDED(a) !strcmp(archive_error_string(a) , "Passphrase required for this entry")
#define PASSWORD_INCORRECT(a) !strcmp(archive_error_string(a) , "Incorrect passphrase")

/*
 * Custom deallocators to be used in Qt smart
 * pointers.
*/
static void deleteArchiveRead(struct archive *ar)
{
    if(ar) {
        archive_read_close(ar);
        archive_read_free(ar);
    }
    return;
}

static void deleteArchiveWrite(struct archive *aw)
{
    if(aw) {
        archive_write_close(aw);
        archive_write_free(aw);
    }
    return;
}

static void ignoreDeleteQFile(QFile *file)
{
    (void)file;
    return;
}


/* Basic string manupilation utilities. */
static char *concat(const char *dest, const char *src)
{
    char *ret = (char*) calloc(sizeof(char), strlen(dest) + strlen(src) + 1);
    strcpy(ret, dest);
    strcat(ret, src);
    return ret;
}

static QString getDirectoryFileName(const QString &dir)
{
    if(dir[dir.count() - 1] == QStringLiteral("/")) {
        return dir.mid(0, dir.count() - 1);
    }
    return dir;
}



DiskExtractorPrivate::DiskExtractorPrivate()
    : DISK_EXTRACTOR_CONSTRUCTOR {
}

DiskExtractorPrivate::~DiskExtractorPrivate()
{
    _mArchiveWrite.clear();
    _mArchiveRead.clear();
    _mArchive.clear();
    _mExtractFilters.clear();
    _mInfo.clear();
}


void DiskExtractorPrivate::setArchive(QFile *archive)
{
    if(_bStarted || _bPaused) {
        return;
    }
    if(archive == nullptr) {
        emit error(InvalidQFile);
        return;
    }
    clear();
    _mArchive = QSharedPointer<QFile>(archive, ignoreDeleteQFile);

    /* Check if exists */
    if(!_mArchive->exists()) {
        _mArchive.clear();
        emit error(ArchiveDoesNotExists);
        return;
    }

    /* Check if readable. */
    if(!_mArchive->isReadable()) {
        _mArchive.clear(); /* delete everything */
        emit error(ArchiveIsNotReadable);
        return;
    }

    /* Check if opened. */
    if(!_mArchive->isOpen()) {
        _mArchive.clear(); /* delete everything */
        emit error(ArchiveIsNotOpened);
        return;
    }
    return;
}

void DiskExtractorPrivate::setArchive(const QString &archivePath)
{
    if(_bStarted || _bPaused) {
        return;
    }
    if(archivePath.isEmpty()) {
        return;
    }
    clear();
    try {
        _mArchive = QSharedPointer<QFile>(new QFile);
    } catch ( ... ) {
        emit error(NotEnoughMemory);
        throw;
    }

    /*
     * Check if its really a file and not a folder.
    */
    if(!QFileInfo(archivePath).isFile()) {
        _mArchive.clear();
        emit error(ArchiveDoesNotExists);
        return;
    }

    _mArchive->setFileName(archivePath);

    /* Check if the file actually exists. */
    if(!_mArchive->exists()) {
        _mArchive.clear(); /* delete everything to avoid futher errors. */
        emit error(ArchiveDoesNotExists);
        return;
    }

    /* Check if we have the permission to read it. */
    auto perm = _mArchive->permissions();
    if(
        !(perm & QFileDevice::ReadUser) &&
        !(perm & QFileDevice::ReadGroup) &&
        !(perm & QFileDevice::ReadOther)
    ) {
        _mArchive.clear();
        emit error(NoPermissionToReadArchive);
        return;
    }

    /*
     * Finally open the file.
    */
    if(!_mArchive->open(QIODevice::ReadOnly)) {
        _mArchive.clear();
        emit error(CannotOpenArchive);
        return;
    }
    return;
}

void DiskExtractorPrivate::setBlockSize(int n)
{
    if(_bStarted || _bPaused) {
        return;
    }
    _nBlockSize = n;
    return;
}

void DiskExtractorPrivate::setOutputDirectory(const QString &destination)
{
    if(_bStarted || _bPaused) {
        return;
    }
    QFileInfo info(destination + "/");
    /* Check if its a directory and not a file , Also check if it exists. */
    if(destination.isEmpty() || !info.isDir() || !QFileInfo::exists(destination)) {
        emit error(InvalidOutputDirectory);
        return;
    }

    /* Check if we have the permission to read and write. */
    if(!info.isWritable() || !info.isReadable()) {
        emit error(NoPermissionToWrite);
        return;
    }
    _mOutputDirectory = info.absoluteFilePath();
    return;
}

void DiskExtractorPrivate::setShowProgress(bool c)
{
    _bNoProgress = !c;
    return;
}

void DiskExtractorPrivate::setPassword(const QString &passwd)
{
    if(passwd.isEmpty()) {
        return;
    }
    _mPassword = passwd;
    return;
}

void DiskExtractorPrivate::addFilter(const QString &filter)
{
    if(_bStarted || _bPaused || filter.isEmpty()) {
        return;
    }
    *(_mExtractFilters.data()) << filter;
    return;
}

void DiskExtractorPrivate::addFilter(const QStringList &filters)
{
    if(_bStarted || _bPaused || filters.isEmpty()) {
        return;
    }
    *(_mExtractFilters.data()) << filters;
    return;
}

void DiskExtractorPrivate::clear()
{
    if(_bStarted || _bPaused) {
        return;
    }
    _mInfo.clear();
    _mInfo = QSharedPointer<QJsonObject>(new QJsonObject);
    _mExtractFilters->clear();
    _mPassword.clear();
    _mOutputDirectory.clear();
    _mArchive.clear();
    _nBlockSize = 10240;
    _nPasswordTriedCountGetInfo = _nPasswordTriedCountExtract = 0;
    _nTotalEntries = -1;
    _bPauseRequested = _bCancelRequested = _bPaused = _bStarted = _bFinished = false;
    return;
}

void DiskExtractorPrivate::getInfo()
{
    if(!_mInfo->isEmpty()) {
        emit info(*(_mInfo.data()));
        return;
    }

    short ret = processArchiveInformation();
    if(!ret) {
        emit info(*(_mInfo.data()));
    } else if(ret == ArchivePasswordIncorrect || ret == ArchivePasswordNeeded) {
        emit getInfoRequirePassword(_nPasswordTriedCountGetInfo);
        ++_nPasswordTriedCountGetInfo;
    } else {
        emit error(ret);
    }
    return;
}

void DiskExtractorPrivate::start()
{
    if(_bStarted || _bPaused || _mArchive.isNull()) {
        return;
    } else if(_nTotalEntries == -1 && !_bNoProgress) {
        short errorCode = getTotalEntriesCount();
        if(_nTotalEntries == -1) {
            if(errorCode == ArchivePasswordIncorrect || errorCode == ArchivePasswordNeeded) {
                emit extractionRequirePassword(_nPasswordTriedCountExtract);
                ++_nPasswordTriedCountExtract;
            } else {
                emit error(errorCode);
                return;
            }
        }
    }


    _bStarted = true;
    emit started();

    short ret = extract();
    if(!ret) {
        _bStarted = false;
        _bFinished = true;
        emit finished();
    } else if(ret == ArchivePasswordIncorrect || ret == ArchivePasswordNeeded) {
        _bStarted = false;
        emit extractionRequirePassword(_nPasswordTriedCountExtract);
        ++_nPasswordTriedCountExtract;
    } else if(ret == OperationCanceled) {
        _bStarted = false;
        emit canceled();
    } else if(ret < 0) {
        _bStarted = false;
        _bPaused = true;
        emit paused();
    } else {
        _bStarted = false;
        emit error(ret);
    }
    return;
}

void DiskExtractorPrivate::pause()
{
    if(_bStarted && !_bPaused) {
        _bPauseRequested = true;
    }
    return;
}

void DiskExtractorPrivate::resume()
{
    if(!_bPaused) {
        return;
    }
    _bPaused = false;
    _bStarted = true;
    emit resumed();

    short ret = extract();
    if(!ret) {
        _bStarted = false;
        _bFinished = true;
        emit finished();
    } else if(ret == ArchivePasswordIncorrect || ret == ArchivePasswordNeeded) {
        _bStarted = false;
        emit extractionRequirePassword(_nPasswordTriedCountExtract);
        ++_nPasswordTriedCountExtract;
    } else if(ret == OperationCanceled) {
        _bStarted = false;
        emit canceled();
    } else if(ret < 0) {
        _bStarted = false;
        _bPaused = true;
        emit paused();
    } else {
        _bStarted = false;
        emit error(ret);
    }

    return;
}

void DiskExtractorPrivate::cancel()
{
    if(_bStarted && !_bPaused && !_bFinished) {
        _bCancelRequested = true;
    }
    return;
}

short DiskExtractorPrivate::extract()
{
    if(_mArchive.isNull()) {
        return ArchiveNotGiven;
    }
    int ret = 0;
    short err = NoError;
    archive_entry *entry = nullptr;

    if(_mArchiveRead.isNull() && _mArchiveWrite.isNull()) {
        _nProcessedEntries = 0;
        /*
         * Reset any previous operations
         * on the file.
        */
        auto prev = _mArchive->pos();
        _mArchive->seek(0);

        _mArchiveRead = QSharedPointer<struct archive>(archive_read_new(), deleteArchiveRead);
        _mArchiveWrite = QSharedPointer<struct archive>(archive_write_disk_new(), deleteArchiveWrite);
        if(!_mPassword.isEmpty()) {
            archive_read_add_passphrase(_mArchiveRead.data(), _mPassword.toLatin1().constData());
        }
        if(!_mArchiveRead.data() && !_mArchiveWrite.data()) {
            _mArchiveRead.clear();
            _mArchiveWrite.clear();
            _mArchive->seek(prev);
            return NotEnoughMemory;
        }
        archive_read_support_format_all(_mArchiveRead.data());
        archive_read_support_filter_all(_mArchiveRead.data());
        if((ret = archive_read_open_fd(_mArchiveRead.data(), _mArchive->handle(),_nBlockSize))) {
            _mArchiveRead.clear();
            _mArchiveWrite.clear();
            _mArchive->seek(prev);
            return ArchiveReadError;
        }

        if((ret = archive_write_disk_set_options(_mArchiveWrite.data(), _nFlags))) {
            _mArchiveRead.clear();
            _mArchiveWrite.clear();
            _mArchive->seek(prev);
            return ArchiveWriteError;
        }

        for (;;) {
            ret = archive_read_next_header(_mArchiveRead.data(), &entry);
            if (ret == ARCHIVE_EOF) {
                break;
            }
            if (ret != ARCHIVE_OK) {
                err = ArchiveCorrupted;
                if(PASSWORD_NEEDED(_mArchiveRead.data())) {
                    err = ArchivePasswordNeeded;
                } else if(PASSWORD_INCORRECT(_mArchiveRead.data())) {
                    err = ArchivePasswordIncorrect;
                }
                _mArchiveRead.clear();
                _mArchiveWrite.clear();
                _mArchive->seek(prev);
                return err;
            }

            if((err = writeData(entry))) {
                _mArchiveRead.clear();
                _mArchiveWrite.clear();
                _mArchive->seek(prev);
                return err;
            }

            if(_nTotalEntries > 0) {
                ++_nProcessedEntries;
                emit progress(QString(archive_entry_pathname(entry)),
                              _nProcessedEntries,
                              _nTotalEntries,
                              (_nProcessedEntries*100)/_nTotalEntries);
            } else {
                emit progress(QString(archive_entry_pathname(entry)), 0, 0, 0);
            }

            QCoreApplication::processEvents();
            if(_bPauseRequested) {
                _bPauseRequested = false;
                return -1;
            }

            if(_bCancelRequested) {
                _bCancelRequested = false;
                _mArchiveRead.clear();
                _mArchiveWrite.clear();
                _mArchive->seek(prev);
                return OperationCanceled;
            }
        }
        _mArchive->seek(prev);
    } else {
        for (;;) {
            ret = archive_read_next_header(_mArchiveRead.data(), &entry);
            if (ret == ARCHIVE_EOF) {
                break;
            }
            if (ret != ARCHIVE_OK) {
                err = ArchiveCorrupted;
                if(PASSWORD_NEEDED(_mArchiveRead.data())) {
                    err = ArchivePasswordNeeded;
                } else if(PASSWORD_INCORRECT(_mArchiveRead.data())) {
                    err = ArchivePasswordIncorrect;
                }
                _mArchiveRead.clear();
                _mArchiveWrite.clear();
                _mArchive->seek(0);
                return err;
            }

            if((err = writeData(entry))) {
                _mArchiveRead.clear();
                _mArchiveWrite.clear();
                _mArchive->seek(0);
                return err;
            }

            if(_nTotalEntries > 0) {
                ++_nProcessedEntries;
                emit progress(QString(archive_entry_pathname(entry)),
                              _nProcessedEntries,
                              _nTotalEntries,
                              (_nProcessedEntries*100)/_nTotalEntries);
            } else {
                emit progress(QString(archive_entry_pathname(entry)), 0, 0, 0);
            }

            /*
             * Process events to know if the user canceled
             * or paused the extraction
            */
            QCoreApplication::processEvents();
            if(_bPauseRequested) {
                _bPauseRequested = false;
                return -1;
            }

            if(_bCancelRequested) {
                _bCancelRequested = false;
                _mArchiveRead.clear();
                _mArchiveWrite.clear();
                _mArchive->seek(0);
                return OperationCanceled;
            }

        }
    }
    /* free memory. */
    _mArchiveRead.clear();
    _mArchiveWrite.clear();
    return NoError;
}

short DiskExtractorPrivate::writeData(struct archive_entry *entry)
{
    if(_mArchiveRead.isNull() || _mArchiveWrite.isNull() || _mArchive.isNull()) {
        return ArchiveNotGiven;
    }

    if(!_mExtractFilters->isEmpty() &&
       !_mExtractFilters->contains(QString(archive_entry_pathname(entry)))) {
        return NoError;
    }

    if(!_mOutputDirectory.isEmpty()) {
        char *new_entry = concat(_mOutputDirectory.toLatin1().constData(), archive_entry_pathname(entry));
        archive_entry_set_pathname(entry, new_entry);
        free(new_entry);
    }

    int ret = archive_write_header(_mArchiveWrite.data(), entry);
    if (ret == ARCHIVE_OK) {
        const void *buff;
        size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
        int64_t offset;
#else
        off_t offset;
#endif
        for (;;) {
            ret = archive_read_data_block(_mArchiveRead.data(), &buff, &size, &offset);
            if (ret == ARCHIVE_EOF) {
                break;
            } else if (ret != ARCHIVE_OK) {
                short err = ArchiveCorrupted;
                if(PASSWORD_NEEDED(_mArchiveRead.data())) {
                    err = ArchivePasswordNeeded;
                } else if(PASSWORD_INCORRECT(_mArchiveRead.data())) {
                    err = ArchivePasswordIncorrect;
                }
                return err;
            } else {
                ret = archive_write_data_block(_mArchiveWrite.data(), buff, size, offset);
                if (ret != ARCHIVE_OK) {
                    return ArchiveWriteError;
                }
            }
        }
    } else {
        return ArchiveHeaderWriteError;
    }

    ret = archive_write_finish_entry(_mArchiveWrite.data());
    if (ret == ARCHIVE_FATAL) {
        return ArchiveHeaderWriteError;
    }
    return NoError;
}

short DiskExtractorPrivate::getTotalEntriesCount()
{
    if(_mArchive.isNull()) {
        return ArchiveNotGiven;
    }
    /*
     * Reset any previous operations
     * on the file.
    */
    auto prev = _mArchive->pos();
    _mArchive->seek(0);

    int ret = 0;
    int count = 0;
    archive_entry *entry = nullptr;
    struct archive *inArchive = archive_read_new();
    if(!_mPassword.isEmpty()) {
        archive_read_add_passphrase(inArchive, _mPassword.toLatin1().constData());
    }
    if(!inArchive) {
        _mArchive->seek(prev);
        return NotEnoughMemory;
    }
    archive_read_support_format_all(inArchive);
    archive_read_support_filter_all(inArchive);
    if((ret = archive_read_open_fd(inArchive, _mArchive->handle(),_nBlockSize))) {
        archive_read_close(inArchive);
        archive_read_free(inArchive);
        _mArchive->seek(prev);
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
            _mArchive->seek(prev);
            return err;
        }
        count += 1;
    }

    _mArchive->seek(prev);
    /* Set total number of entries. */
    _nTotalEntries = count;

    /* free memory. */
    archive_read_close(inArchive);
    archive_read_free(inArchive);
    return NoError;
}

short DiskExtractorPrivate::processArchiveInformation()
{
    if(_mArchive.isNull()) {
        return ArchiveNotGiven;
    }

    /*
     * Reset any previous operations
     * on the file.
    */
    auto prev = _mArchive->pos();
    _mArchive->seek(0);

    int ret = 0;
    archive_entry *entry = nullptr;
    struct archive *inArchive = archive_read_new();
    if(!_mPassword.isEmpty()) {
        archive_read_add_passphrase(inArchive, _mPassword.toLatin1().constData());
    }
    if(!inArchive) {
        _mArchive->seek(prev);
        return NotEnoughMemory;
    }
    archive_read_support_format_all(inArchive);
    archive_read_support_filter_all(inArchive);
    if((ret = archive_read_open_fd(inArchive, _mArchive->handle(),_nBlockSize))) {
        archive_read_close(inArchive);
        archive_read_free(inArchive);
        _mArchive->seek(prev);
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
            _mArchive->seek(prev);
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
        qint64 blocks = (qint64) ceil(entry_stat->st_size / blockSizeInBytes);
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
        _mInfo->insert(CurrentFile, CurrentEntry);
    }

    _mArchive->seek(prev);

    /* Set total number of entries. */
    _nTotalEntries = _mInfo->size();

    /* free memory. */
    archive_read_close(inArchive);
    archive_read_free(inArchive);
    return NoError;
}
