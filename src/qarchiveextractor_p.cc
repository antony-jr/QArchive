#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>

#include "qarchiveextractor_p.hpp"
#include "qarchive_enums.hpp"

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

// Helpful macros to check if an archive error is caused due to
// faulty passwords.
// Expects a pointer to a struct archive , returns 1 if password
// is needed or incorrect.
#define PASSWORD_NEEDED(a) !qstrcmp(archive_error_string(a) ,"Passphrase required for this entry")
#define PASSWORD_INCORRECT(a) !qstrcmp(archive_error_string(a) , "Incorrect passphrase")

using namespace QArchive;

/// MutableMemoryFile class provides a memory files which has both setters and getters
/// unlike MemoryFile which only has getters.
/// This will force the users to not mess up the integrity of a MemoryFile like
/// deleting the internal pointers which will be automatically freed by MemoryFile
/// destructor.
void MutableMemoryFile::setFileInformation(const QJsonObject &info) {
    m_FileInformation = info;
}

void MutableMemoryFile::setBuffer(QBuffer *buffer) {
    m_Buffer = std::make_shared<QBuffer>(buffer);
}

QJsonObject MutableMemoryFile::getFileInformation() const {
    return m_FileInformation;
}

std::shared_ptr<QBuffer> MutableMemoryFile::getBuffer() const {
    return m_Buffer;
}
/// ---

namespace QArchive {
class ArchiveFilter {
  public:
    ArchiveFilter()
        : m_match(archive_match_new(), archive_match_free) {
    }

    short addIncludePattern(const QString& pattern) {
        if (archive_match_include_pattern_w(m_match.data(), pattern.toStdWString().c_str()) != ARCHIVE_OK)
            return ApplyPatternFailed;
        return NoError;
    }

    short addIncludePatterns(const QStringList& includePatterns) {
        for (const auto& pattern : includePatterns) {
            auto ret = addIncludePattern(pattern);
            if (ret != NoError)
                return ret;
        }
        return NoError;
    }

    short addExcludePattern(const QString& pattern) {
        if (archive_match_exclude_pattern_w(m_match.data(), pattern.toStdWString().c_str()) != ARCHIVE_OK)
            return ApplyPatternFailed;
        return NoError;
    }

    short addExcludePatterns(const QStringList& excludePatterns) {
        for (const auto& pattern : excludePatterns) {
            auto ret = addExcludePattern(pattern);
            if (ret != NoError)
                return ret;
        }
        return NoError;
    }

    bool isEntryExcluded(struct archive_entry* entry) const {
        return archive_match_excluded(m_match.data(), entry);
    }

  private:
    QSharedPointer<struct archive> m_match;

};
}  // namespace QArchive

static QJsonObject getArchiveEntryInformation(archive_entry *entry, bool bExcluded) {
    QJsonObject CurrentEntry;
    auto CurrentFile = QString(archive_entry_pathname(entry));

    qint64 size = archive_entry_size(entry);
    qint64 roundedSize = size;
    QString sizeUnits = "Bytes";
    if(roundedSize == 0) {
        sizeUnits = "None";
        roundedSize = 0;
    } else if(roundedSize < 1024) {
        sizeUnits = "Bytes";
    } else if(roundedSize >= 1024 && roundedSize < 1048576) {
        sizeUnits = "KiB";
        roundedSize /= 1024;
    } else if(roundedSize >= 1048576 && roundedSize < 1073741824) {
        sizeUnits = "MiB";
        roundedSize /= 1048576;
    } else {
        sizeUnits = "GiB";
        roundedSize /= 1073741824;
    }

    // The libarchive neither populates st_blksize nor st_blocks. So, these values are useless. Use 512 for all cases as a very likely value.
    qint64 blockSizeInBytes = 512;
    qint64 blocks = size / blockSizeInBytes + (size % blockSizeInBytes > 0 ? 1 : 0); // add one more block if there is a remainder

    auto lastAccessT = archive_entry_atime(entry);
    auto lastModT = archive_entry_mtime(entry);
    auto lastStatusModT = archive_entry_ctime(entry);

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
    }

    QFile fileInfo(CurrentFile);
    // Set the values.
    if(FileType != "RegularFile") {
        CurrentEntry.insert("FileName", getDirectoryFileName(CurrentFile));
    } else {
        CurrentEntry.insert("FileName", fileInfo.fileName());
    }

    CurrentEntry.insert("FileType", QJsonValue(FileType));
    CurrentEntry.insert("RawSize", QJsonValue(size));
    CurrentEntry.insert("Size", QJsonValue(roundedSize));
    CurrentEntry.insert("SizeUnit", sizeUnits);
    CurrentEntry.insert("BlockSize", QJsonValue(blockSizeInBytes));
    CurrentEntry.insert("BlockSizeUnit", "Bytes");
    CurrentEntry.insert("Blocks", QJsonValue(blocks));
    if(lastAccessT) {
        CurrentEntry.insert("LastAccessedTime",
                            QJsonValue(
                                (
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
                                    QDateTime::fromSecsSinceEpoch(lastAccessT)
#else
                                    QDateTime::fromTime_t(lastAccessT)
#endif
                                ).toString(Qt::ISODate)));
    } else {
        CurrentEntry.insert("LastAccessedTime", "Unknown");
    }

    if(lastModT) {
        CurrentEntry.insert("LastModifiedTime",
                            QJsonValue((
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
                                           QDateTime::fromSecsSinceEpoch(lastModT)
#else
                                           QDateTime::fromTime_t(lastModT)
#endif
                                       ).toString(Qt::ISODate)));
    } else {
        CurrentEntry.insert("LastModifiedTime", "Unknown");
    }

    if(lastStatusModT) {
        CurrentEntry.insert("LastStatusModifiedTime",
                            QJsonValue((
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
                                           QDateTime::fromSecsSinceEpoch(lastStatusModT)
#else
                                           QDateTime::fromTime_t(lastStatusModT)
#endif
                                       ).toString(Qt::ISODate)));
    } else {
        CurrentEntry.insert("LastStatusModifiedTime", "Unknown");
    }

    CurrentEntry.insert("Excluded", bExcluded);

    return CurrentEntry;
}

// ExtractorPrivate constructor constructs the object which is the private class
// implementation to the DiskExtractor.
// This class is responsible for extraction and information retrival of the data
// inside an archive.
ExtractorPrivate::ExtractorPrivate(bool memoryMode)
    : b_MemoryMode(memoryMode)
    , n_Flags(ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_SECURE_NODOTDOT) {
#ifdef __cpp_lib_make_unique
    m_archiveFilter = std::make_unique<ArchiveFilter>();
#else
    m_archiveFilter.reset(new ArchiveFilter);
#endif

    if(b_MemoryMode) {
#ifdef __cpp_lib_make_unique
        m_ExtractedFiles = std::make_unique<QVector<MemoryFile>>();
#else
        m_ExtractedFiles.reset(new QVector<MemoryFile>);
#endif
    }
}

ExtractorPrivate::~ExtractorPrivate() {
    clear();
}

// Sets the given pointer to QIODevice as the Archive file itself.
void ExtractorPrivate::setArchive(QIODevice *archive) {
    if(b_Started || b_Paused) {
        return;
    }
    clear();
    m_Archive = archive;
}

// Sets the archive path as the given QString which will be later
// opened to be used as the Archive file.
void ExtractorPrivate::setArchive(const QString &archivePath) {
    if(b_Started || b_Paused || archivePath.isEmpty()) {
        return;
    }
    clear();
    m_ArchivePath = archivePath;
}

// Blocksize to be used when extracting the given archive.
void ExtractorPrivate::setBlockSize(int n) {
    if(b_Started || b_Paused) {
        return;
    }
    n_BlockSize = n;
}

// Sets the directory where the extraction data to be extracted.
void ExtractorPrivate::setOutputDirectory(const QString &destination) {
    if(b_MemoryMode || b_Started || b_Paused || destination.isEmpty()) {
        return;
    }
    m_OutputDirectory = destination + "/";
}

// Enables/Disables the progress of the extraction with respect to the
// given bool.
void ExtractorPrivate::setCalculateProgress(bool c) {
    b_NoProgress = !c;
}

// Sets the password for the archive when extracting the data.
// This method should be accessible even if the extractor is started
// since the user may set password anytime.
void ExtractorPrivate::setPassword(const QString &passwd) {
#if ARCHIVE_VERSION_NUMBER >= 3003003
    if(passwd.isEmpty()) {
        return;
    }
    m_Password = passwd;
#else
    (void)passwd;
#endif
}

// Adds extract filters , if set , only the files in the filter
// will be extracted , the filter has to correspond to the exact
// path given in the archive.
void ExtractorPrivate::addFilter(const QString &filter) {
    if(b_Started || b_Paused || filter.isEmpty()) {
        return;
    }
    m_ExtractFilters << filter;
}

// Overload of addFilter to accept list of QStrings.
void ExtractorPrivate::addFilter(const QStringList &filters) {
    if(b_Started || b_Paused || filters.isEmpty()) {
        return;
    }
    m_ExtractFilters << filters;
}

void ExtractorPrivate::addIncludePattern(const QString &pattern) {
    if(b_Started || b_Paused || pattern.isEmpty()) {
        return;
    }
    auto errorCode = m_archiveFilter->addIncludePattern(pattern);
    if (errorCode != NoError)
        emit error(errorCode);
}

void ExtractorPrivate::addIncludePattern(const QStringList &patterns) {
    if(b_Started || b_Paused || patterns.isEmpty()) {
        return;
    }
    auto errorCode = m_archiveFilter->addIncludePatterns(patterns);
    if (errorCode != NoError)
        emit error(errorCode);
}

void ExtractorPrivate::addExcludePattern(const QString &pattern) {
    if(b_Started || b_Paused || pattern.isEmpty()) {
        return;
    }
    auto errorCode = m_archiveFilter->addExcludePattern(pattern);
    if (errorCode != NoError)
        emit error(errorCode);
}

void ExtractorPrivate::addExcludePattern(const QStringList &patterns) {
    if(b_Started || b_Paused || patterns.isEmpty()) {
        return;
    }
    auto errorCode = m_archiveFilter->addExcludePatterns(patterns);
    if (errorCode != NoError)
        emit error(errorCode);
}

void ExtractorPrivate::setBasePath(const QString& path) {
    if (b_Started || b_Paused) {
        return;
    }
    m_basePath.setPath("/" + path);
    b_hasBasePath = !path.isEmpty();
}

void ExtractorPrivate::setRawMode(bool enabled)
{
    if (b_Started || b_Paused) {
        return;
    }
    b_RawMode = enabled;
}

void ExtractorPrivate::setRawOutputFilename(const QString& name)
{
    if (b_Started || b_Paused || b_MemoryMode) {
        return;
    }
    m_RawOutputFilename = name;
    b_RawMode = !name.isEmpty();
}

// Clears all internal data and sets it back to default.
void ExtractorPrivate::clear() {
    if(b_Started) {
        return;
    }
    n_BlockSize = 10240;
    n_PasswordTriedCountGetInfo = n_PasswordTriedCountExtract = 0;
    n_TotalEntries = -1;
    b_RawMode = b_ProcessingArchive = b_StartRequested = false;
    b_PauseRequested = b_CancelRequested = b_Paused = b_Started = b_Finished = b_ArchiveOpened = false;

    n_BytesTotal = 0;

    m_ArchivePath.clear();
#if ARCHIVE_VERSION_NUMBER >= 3003003
    m_Password.clear();
#endif
    m_OutputDirectory.clear();
    m_RawOutputFilename.clear();
    m_ArchiveRead.clear();
    m_ArchiveWrite.clear();
    m_Info = {};
    m_ExtractFilters.clear();
#ifdef __cpp_lib_make_unique
    m_archiveFilter = std::make_unique<ArchiveFilter>();
#else
    m_archiveFilter.reset(new ArchiveFilter);
#endif

    if(b_MemoryMode) {
#ifdef __cpp_lib_make_unique
        m_ExtractedFiles = std::make_unique<QVector<MemoryFile>>();
#else
        m_ExtractedFiles.reset(new QVector<MemoryFile>);
#endif
    }

    if(b_QIODeviceOwned) {
        m_Archive->close();
        m_Archive->deleteLater();
    } else {
        m_Archive = nullptr;
    }
    m_CurrentArchiveEntry = nullptr;
    b_QIODeviceOwned = false;
}

// Returns the information of the archive through info signal.
void ExtractorPrivate::getInfo() {
    if(b_ProcessingArchive) {
        return;
    }

    b_ProcessingArchive = true;

    if(!m_Info.isEmpty()) {
        b_ProcessingArchive = false;
        emit info(m_Info);
        if(b_StartRequested) {
            b_StartRequested = false;
            start();
        }
        return;
    }

    // Open the Archive.
    short errorCode = openArchive();
    if(errorCode != NoError) {
        b_ProcessingArchive = false;
        emit error(errorCode);
        if(b_StartRequested) {
            b_StartRequested = false;
            start();
        }
        return;
    }

    errorCode = processArchiveInformation();
    b_ProcessingArchive = false;

    if(!errorCode) {
        emit info(m_Info);
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


    if(b_StartRequested) {
        b_StartRequested = false;
        start();
    }
}

void ExtractorPrivate::start() {
    if(b_Started || b_Paused || b_ProcessingArchive) {
        if(!b_StartRequested) {
            b_StartRequested = true;
        }
        return;
    }

    // Open the Archive.
    short errorCode = openArchive();
    if(errorCode != NoError) {
        emit error(errorCode);
        return;
    }

    // Check and Set Output Directory.
    // If it's not memory mode.
    if(!b_MemoryMode && !m_OutputDirectory.isEmpty()) {
        errorCode = checkOutputDirectory();
        if(errorCode != NoError) {
            emit error(errorCode );
            return;
        }
    }

    // All Okay then start the extraction.
    b_Started = true;
    b_Finished = false;
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
            emit error(errorCode);
            return;
        }
    }

    errorCode = extract();
    if(errorCode == NoError) {
        b_Started = false;
        b_Finished = true;
        m_Archive->close();
        if(!b_MemoryMode) {
            emit diskFinished();
        } else {
            emit memoryFinished(new MemoryExtractorOutput(std::move(m_ExtractedFiles)));
#ifdef __cpp_lib_make_unique
            m_ExtractedFiles = std::make_unique<QVector<MemoryFile>>();
#else
            m_ExtractedFiles.reset(new QVector<MemoryFile>);
#endif
        }
    }
#if ARCHIVE_VERSION_NUMBER >= 3003003
    else if(errorCode == ArchivePasswordIncorrect || errorCode == ArchivePasswordNeeded) {
        b_Started = false;
        emit extractionRequirePassword(n_PasswordTriedCountExtract);
        ++n_PasswordTriedCountExtract;
        emit error(errorCode);
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
}

// Pauses the extractor.
void ExtractorPrivate::pause() {
    if(b_Started && !b_Paused) {
        b_PauseRequested = true;
    }
}

// Resumes the extractor.
void ExtractorPrivate::resume() {
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
        if(!b_MemoryMode) {
            emit diskFinished();
        } else {
            emit memoryFinished(new MemoryExtractorOutput(std::move(m_ExtractedFiles)));
#ifdef __cpp_lib_make_unique
            m_ExtractedFiles = std::make_unique<QVector<MemoryFile>>();
#else
            m_ExtractedFiles.reset(new QVector<MemoryFile>);
#endif
        }
    }
#if ARCHIVE_VERSION_NUMBER >= 3003003
    else if(ret == ArchivePasswordIncorrect || ret == ArchivePasswordNeeded) {
        b_Started = false;
        emit extractionRequirePassword(n_PasswordTriedCountExtract);
        ++n_PasswordTriedCountExtract;
        emit error(ret);
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
}

// Cancels the extraction.
void ExtractorPrivate::cancel() {
    if(b_Started && !b_Paused && !b_Finished) {
        b_CancelRequested = true;
    }
}


short ExtractorPrivate::openArchive() {
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
        m_Archive = file;
    } else {
        /// Open the archive if it's not opened.
        if(!m_Archive->isOpen() && !m_Archive->open(QIODevice::ReadOnly)) {
            return ArchiveIsNotOpened;
        }

        if(!m_Archive->isReadable()) { // Check if it is readable.
            return ArchiveIsNotReadable;
        }
    }
    b_ArchiveOpened = true;
    return NoError;
}

short ExtractorPrivate::checkOutputDirectory() {
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

short ExtractorPrivate::extract() {
    if(m_Archive == nullptr) {
        return ArchiveNotGiven;
    }
    int ret = 0;
    short err = NoError;
    archive_entry *entry = nullptr;

    if(m_ArchiveRead.isNull() && (b_MemoryMode || m_ArchiveWrite.isNull())) {
        n_ProcessedEntries = 0;
        n_BytesProcessed = 0;

        m_ArchiveRead = QSharedPointer<struct archive>(archive_read_new(), ArchiveReadDestructor);
        if(!b_MemoryMode) {
            m_ArchiveWrite = QSharedPointer<struct archive>(archive_write_disk_new(), ArchiveWriteDestructor);
            if(!m_ArchiveRead.data() || !m_ArchiveWrite.data()) {
                m_ArchiveRead.clear();
                m_ArchiveWrite.clear();
                return NotEnoughMemory;
            }
        } else if(!m_ArchiveRead.data()) {
            m_ArchiveRead.clear();
            return NotEnoughMemory;
        }

#if ARCHIVE_VERSION_NUMBER >= 3003003
        if(!m_Password.isEmpty()) {
            archive_read_add_passphrase(m_ArchiveRead.data(), m_Password.toUtf8().constData());
        }
#endif
        toggleArchiveFormat(m_ArchiveRead.data());
        archive_read_support_filter_all(m_ArchiveRead.data());

        if(archiveReadOpenQIODevice(m_ArchiveRead.data(), n_BlockSize, m_Archive)) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            return ArchiveReadError;
        }

        if(!b_MemoryMode && archive_write_disk_set_options(m_ArchiveWrite.data(), n_Flags)) {
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            return ArchiveWriteError;
        }

    }
    for (;;) {
        if(m_CurrentArchiveEntry) {
            err = writeData(m_CurrentArchiveEntry);
            if(err == OperationPaused) {
                return err;
            } else if(err) { // NoError = 0
                m_ArchiveRead.clear();
                m_ArchiveWrite.clear();
                return err;
            }
            ++n_ProcessedEntries;

            // Report final progress signal after extracting the file fully.
            if(n_BytesTotal > 0 && n_TotalEntries > 0) {
                emit progress(QString(archive_entry_pathname(m_CurrentArchiveEntry)),
                              n_ProcessedEntries,
                              n_TotalEntries,
                              n_BytesProcessed, n_BytesTotal);
            } else {
                emit progress(QString(archive_entry_pathname(m_CurrentArchiveEntry)),
                              1,
                              1,
                              1,
                              1);

            }

            archive_entry_clear(m_CurrentArchiveEntry);
            m_CurrentArchiveEntry = nullptr;
        }
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
        if(err == OperationPaused) {
            return err;
        } else if(err) { // NoError = 0
            m_ArchiveRead.clear();
            m_ArchiveWrite.clear();
            return err;
        }
        ++n_ProcessedEntries;

        // Report final progress signal after extracting the file fully.
        if(n_BytesTotal > 0 && n_TotalEntries > 0) {
            emit progress(QString(archive_entry_pathname(entry)),
                          n_ProcessedEntries,
                          n_TotalEntries,
                          n_BytesProcessed, n_BytesTotal);
        } else {
            emit progress(QString(archive_entry_pathname(entry)),
                          1,
                          1,
                          1,
                          1);

        }
        archive_entry_clear(entry);
        QCoreApplication::processEvents(); // call event loop for the signal to take effect.
    }

    /* free memory. */
    m_ArchiveRead.clear();
    m_ArchiveWrite.clear();
    return NoError;
}

void ExtractorPrivate::toggleArchiveFormat(struct archive* inArchive)
{
    if (b_RawMode) {
        archive_read_support_format_raw(inArchive);
        archive_read_support_format_empty(inArchive);
    } else {
        archive_read_support_format_all(inArchive);
    }
}

short ExtractorPrivate::writeData(struct archive_entry *entry) {
    if(m_ArchiveRead.isNull() || (!b_MemoryMode && m_ArchiveWrite.isNull()) || m_Archive == nullptr) {
        return ArchiveNotGiven;
    }

    if((!m_ExtractFilters.isEmpty() &&
            !m_ExtractFilters.contains(QString(archive_entry_pathname(entry))))
            || (m_archiveFilter->isEntryExcluded(entry))) {
        n_BytesProcessed += archive_entry_size(entry);
        return NoError;
    }

    if (!b_MemoryMode && b_RawMode && !m_RawOutputFilename.isEmpty()) {
        const auto& path = (QFileInfo(archive_entry_pathname(entry)).path() + QString::fromLatin1("/") + m_RawOutputFilename).toStdWString();
        archive_entry_copy_pathname_w(entry, path.c_str());
    }
    if(b_hasBasePath) {
        const auto& relativePath = m_basePath.relativeFilePath(QString::fromLatin1("/") + archive_entry_pathname(entry)).toStdWString();
        if (relativePath == L".") // Root directory
            return NoError;
        archive_entry_copy_pathname_w(entry, relativePath.c_str());
    }
    if(!b_MemoryMode && !m_OutputDirectory.isEmpty()) {
        QDir outDir(m_OutputDirectory);
        const auto& new_entry = outDir.absoluteFilePath(QString::fromStdWString(archive_entry_pathname_w(entry))).toStdWString();
        archive_entry_copy_pathname_w(entry, new_entry.c_str());
    }

    MutableMemoryFile currentNode;
    int ret = ARCHIVE_OK;
    if(m_CurrentArchiveEntry != entry) {
        if(!b_MemoryMode) {
            ret = archive_write_header(m_ArchiveWrite.data(), entry);
        } else {
            currentNode.setFileInformation(getArchiveEntryInformation(entry, false));

            /// Skip Directories.
            if((currentNode.getFileInformation()).value("FileType").toString() == "Directory") {
                return NoError;
            }

            currentNode.setBuffer(new QBuffer);

            if(!(currentNode.getBuffer())->open(QIODevice::ReadWrite)) {
                return ArchiveHeaderWriteError;
            }

            m_CurrentMemoryFile = currentNode;
        }
    } else {
        currentNode = m_CurrentMemoryFile;
    }
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
                if(!b_MemoryMode) {
                    ret = archive_write_data_block(m_ArchiveWrite.data(), buff, size, offset);
                    if (ret != ARCHIVE_OK) {
                        return ArchiveWriteError;
                    }
                } else {
                    (currentNode.getBuffer())->seek(offset);
                    if((currentNode.getBuffer())->write(static_cast<const char*>(buff), size) == -1) {
                        return ArchiveWriteError;
                    }
                }
                n_BytesProcessed += size;
                if(n_BytesTotal > 0 && n_TotalEntries > 0) {
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
                m_CurrentArchiveEntry = entry;
                return OperationPaused;
            } else if(b_CancelRequested) {
                b_CancelRequested = false;
                return OperationCanceled;
            }

        }
    } else {
        return ArchiveHeaderWriteError;
    }

    if(!b_MemoryMode) {
        ret = archive_write_finish_entry(m_ArchiveWrite.data());
        if (ret == ARCHIVE_FATAL) {
            return ArchiveHeaderWriteError;
        }
    } else {
        (currentNode.getBuffer())->close();
        m_ExtractedFiles->append(
            MemoryFile(
                currentNode.getFileInformation(),
                currentNode.getBuffer()));
        m_CurrentMemoryFile = MutableMemoryFile();
    }
    return NoError;
}

short ExtractorPrivate::getTotalEntriesCount() {
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

    toggleArchiveFormat(inArchive);
    archive_read_support_filter_all(inArchive);
    if(archiveReadOpenQIODevice(inArchive, n_BlockSize, m_Archive)) {
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

short ExtractorPrivate::processArchiveInformation() {
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
    toggleArchiveFormat(inArchive);
    archive_read_support_filter_all(inArchive);

    if(archiveReadOpenQIODevice(inArchive, n_BlockSize, m_Archive)) {
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
        auto CurrentFile = QString(archive_entry_pathname(entry));
        QJsonObject CurrentEntry = getArchiveEntryInformation(entry, m_archiveFilter->isEntryExcluded(entry));
        m_Info.insert(CurrentFile, CurrentEntry);
        n_BytesTotal += archive_entry_size(entry);
        // Clear the entry since it is re-used by the libarchive internally that may lead to the stale data be taken.
        archive_entry_clear(entry);
        QCoreApplication::processEvents();
    }

    // set total number of entries.
    n_TotalEntries = m_Info.size();

    // free memory.
    archive_read_close(inArchive);
    archive_read_free(inArchive);
    return NoError;
}
