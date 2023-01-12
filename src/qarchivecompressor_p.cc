#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QFileInfo>

#include <deque>

#include "qarchive_enums.hpp"
#include "qarchivecompressor_p.hpp"

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
}

using namespace QArchive;

namespace {
bool contains(const QString& entry, const std::deque<CompressorPrivate::Node*>& vec)
{
    return std::any_of(vec.begin(), vec.end(), [&](const CompressorPrivate::Node* n) {
        return n && n->valid && n->entry == entry;
    });
}
} // namespace

// Node is a private structure which is used store info about entries to be
// compressed by the Compressor.
short CompressorPrivate::Node::open() {
    if(valid) {
        return NoError;
    }

    if (path.isEmpty() && (!io && entry.isEmpty())) {
        return FileDoesNotExist;
    }

    if (path.isEmpty() && io) {
        isInMemory = true;
        if(!io->isOpen() &&
                !io->open(QIODevice::ReadOnly)) {
            return CannotOpenFile;
        }

        if(!io->isReadable()) {
            return NoPermissionToReadFile;
        }

        if(io->isSequential()) {
            return IODeviceSequential;
        }
    }

    valid = true;
    return NoError;
}

// CompressorPrivate is the private class which handles the
// compression of data to disk.
// It can compress data in all formats of archive supported by libarchive.
// However there is a method to set password for archives , Only
// ZIP Format is currently supported for encrypting archives with a user given.
CompressorPrivate::CompressorPrivate(bool memoryMode)
    : b_MemoryMode(memoryMode) {
    if(!b_MemoryMode) {
#ifdef __cpp_lib_make_unique
        m_TemporaryFile = std::make_unique<QSaveFile>();
#else
        m_TemporaryFile.reset(new QSaveFile);
#endif
    } else {
#ifdef __cpp_lib_make_unique
        m_Buffer = std::make_unique<QBuffer>();
#else
        m_Buffer.reset(new QBuffer);
#endif
    }
}

CompressorPrivate::~CompressorPrivate() {
    clear();
}

// Set the filename for the archive to written on disk , the filename can
// be relative or absolute as long it is not existing already or does
// not have the permission to write.
void CompressorPrivate::setFileName(const QString &fileName) {
    if(b_MemoryMode || b_Started || b_Paused) {
        return;
    }
    m_TemporaryFile->setFileName(fileName);
}

// Explicitly set the format of the archive , when the format is explicitly set
// then even if the filename corresponds to some other format , the compressor will
// only compress the data in the given archive format.
void CompressorPrivate::setArchiveFormat(short format) {
    if(b_Started || b_Paused) {
        return;
    }
    m_ArchiveFormat = format;
}

// Only used for ZIP Archives , other formats are ignored.
void CompressorPrivate::setPassword(const QString &passwd) {
#if ARCHIVE_VERSION_NUMBER >= 3003003
    if(b_Started || b_Paused) {
        return;
    }
    m_Password = passwd;
#else
    (void)passwd;
#endif
}

void CompressorPrivate::setBlockSize(int size) {
    if(b_Started || b_Paused) {
        return;
    }
    n_BlockSize = size;
}

void CompressorPrivate::addFiles(const QString &entryName, QIODevice *device) {
    if(b_Started || b_Paused) {
        return;
    }

    auto node = new Node;
    node->entry = entryName;
    node->io = device;
    m_StaggedFiles.push_back(node);
}

void CompressorPrivate::addFiles(const QStringList &entries, const QVariantList &devices) {
    if(b_Started || b_Paused) {
        return;
    }

    if(entries.size() != devices.size()) {
        return;
    }

    for(auto i = 0; i < entries.size(); ++i) {
        auto node = new Node;
        node->entry = entries.at(i);
        node->io = devices.at(i).value<QIODevice*>();
        m_StaggedFiles.push_back(node);
    }
}

void CompressorPrivate::addFiles(const QString &file) {
    if(b_Started || b_Paused) {
        return;
    }

    QFileInfo info(file);
    if(contains(info.fileName(), m_StaggedFiles)) {
        return;
    }

    auto node = new Node;
    node->path = file;
    node->entry = info.fileName();
    m_StaggedFiles.push_back(node);
}

void CompressorPrivate::addFiles(const QStringList &files) {
    if(b_Started || b_Paused) {
        return;
    }
    for(const auto& file : files) {
        QFileInfo info(file);
        if(contains(info.fileName(), m_StaggedFiles)) {
            continue;
        }

        auto node = new Node;
        node->path = file;
        node->entry = info.fileName();
        m_StaggedFiles.push_back(node);
    }
}

// Adds a single file and uses a custom entry name with
// respect to the given data.
void CompressorPrivate::addFiles(const QString &entryName, const QString &file) {
    if(b_Started || b_Paused) {
        return;
    }
    if(contains(entryName, m_StaggedFiles)) {
        return;
    }

    auto node = new Node;
    node->path = file;
    node->entry = entryName;
    m_StaggedFiles.push_back(node);
}

// Adds multiple files and uses a corresponding list of
// entry names with respect to the given data.
void CompressorPrivate::addFiles(const QStringList &entryNames, const QStringList &files) {
    if(b_Started || b_Paused) {
        return;
    }
    if(entryNames.size() != files.size()) {
        return;
    }
    for(auto i = 0; i < files.size(); ++i) {
        if(contains(entryNames.at(i), m_StaggedFiles)) {
            continue;
        }

        auto node = new Node;
        node->path = files.at(i);
        node->entry = entryNames.at(i);
        m_StaggedFiles.push_back(node);
    }
}

void CompressorPrivate::removeFiles(const QString &entry) {
    if (b_Started || b_Paused) {
        return;
    }

    auto it = std::find_if(m_StaggedFiles.begin(), m_StaggedFiles.end(), [&](const Node* f){ return f && f->entry == entry; });
    if (it != m_StaggedFiles.end()) {
        m_StaggedFiles.erase(it);
    }
}

void CompressorPrivate::removeFiles(const QStringList &entries) {
    if (b_Started || b_Paused) {
        return;
    }

    std::for_each(entries.begin(), entries.end(), [this](const QString& e){ removeFiles(e); });
}

Q_DECL_DEPRECATED void CompressorPrivate::removeFiles(const QString &entryName, const QString &) {
    removeFiles(entryName);
}

Q_DECL_DEPRECATED void CompressorPrivate::removeFiles(const QStringList &entryNames, const QStringList &) {
    removeFiles(entryNames);
}

/* clears internal cache. */
void CompressorPrivate::clear() {
    if(b_Started) {
        return;
    }
    b_PauseRequested = b_CancelRequested = b_Paused = b_Started = b_Finished = false;
#if ARCHIVE_VERSION_NUMBER >= 3003003
    m_Password.clear();
#endif

    m_ArchiveFormat = 0;
    n_BlockSize = 10240;

    // TODO: do we need to reset n_BytesTotal here?
    n_BytesProcessed = 0;
    n_BytesTotal = 0;

    qDeleteAll(m_ConfirmedFiles);
    m_ConfirmedFiles.clear();
    qDeleteAll(m_StaggedFiles);
    m_StaggedFiles.clear();

    if(!b_MemoryMode) {
#ifdef __cpp_lib_make_unique
        m_TemporaryFile = std::make_unique<QSaveFile>();
#else
        m_TemporaryFile.reset(new QSaveFile);
#endif
    } else {
#ifdef __cpp_lib_make_unique
        m_Buffer = std::make_unique<QBuffer>();
#else
        m_Buffer.reset(new QBuffer);
#endif
    }
}

// Starts the compression.
void CompressorPrivate::start() {
    if(b_Started || b_Paused) {
        return;
    }
    if (!b_MemoryMode && m_TemporaryFile->fileName().isEmpty()) {
        emit error(ArchiveFileNameNotGiven, {});
        return;
    }
    if (!b_MemoryMode && QFileInfo::exists(m_TemporaryFile->fileName())) {
        emit error(ArchiveFileAlreadyExists, m_TemporaryFile->fileName());
        return;
    }
    if (m_StaggedFiles.empty()) {
        emit error(NoFilesToCompress, !b_MemoryMode ? m_TemporaryFile->fileName() : "");
        return;
    }

    // Guess Archive Format if not given.
    if(!m_ArchiveFormat && !guessArchiveFormat()) { // if ArchiveFormat == 0 then no format is set.
        m_ArchiveFormat = ZipFormat; // Default format.
    }

    /// Confirm files.
    n_BytesTotal = 0;
    if(!confirmFiles()) {
        return;
    }

    b_Started = true;
    b_Finished = false;
    emit started();

    n_BytesProcessed = 0;

    short ret = compress();
    if(ret == NoError) {
        b_Started = false;
        b_Finished = true;
        if(b_MemoryMode) {
            emit memoryFinished(m_Buffer.release());
#ifdef __cpp_lib_make_unique
            m_Buffer = std::make_unique<QBuffer>();
#else
            m_Buffer.reset(new QBuffer);
#endif
        } else {
            m_TemporaryFile->commit();
            emit diskFinished();
        }
    } else if(ret == OperationCanceled) {
        b_Started = false;
        emit canceled();
    } else if(ret == OperationPaused) {
        b_Started = false;
        b_Paused = true;
        emit paused();
    }
}

void CompressorPrivate::resume() {
    if(!b_Paused || b_Finished) {
        return;
    }
    b_Paused = false;
    b_Started = true;
    emit resumed();

    short ret = compress();
    if(ret == NoError) {
        b_Started = false;
        b_Finished = true;
        if(b_MemoryMode) {
            emit memoryFinished(m_Buffer.release());
#ifdef __cpp_lib_make_unique
            m_Buffer = std::make_unique<QBuffer>();
#else
            m_Buffer.reset(new QBuffer);
#endif
        } else {
            m_TemporaryFile->commit();
            emit diskFinished();
        }
    } else if(ret == OperationCanceled) {
        b_Started = false;
        emit canceled();
    } else if(ret == OperationPaused) {
        b_Started = false;
        b_Paused = true;
        emit paused();
    } else {
        b_Started = false;
    }
}

void CompressorPrivate::pause() {
    if(!b_Started || b_Finished || b_Paused) {
        return;
    }
    b_PauseRequested = true;
}

// Cancels the current compression process, if the compression process is
// paused then the compression cannot be canceled.
void CompressorPrivate::cancel() {
    if(!b_Started || b_Finished || b_Paused) {
        return;
    }
    b_CancelRequested = true;
}

// Guesses the archive format from the given archive filename , on success
// this returns true or vice-versa.
bool CompressorPrivate::guessArchiveFormat() {
    if(b_MemoryMode) {
        return false;
    }

    if (m_TemporaryFile->fileName().isEmpty()) {
        return false;
    }

    auto ext = QFileInfo(m_TemporaryFile->fileName()).suffix().toLower();
    m_ArchiveFormat = [ext] {
        if (ext == "bz") {
            return BZipFormat;
        }
        if (ext == "bz2") {
            return BZip2Format;
        }
        if (ext == "gz") {
            return GZipFormat;
        }
        if (ext == "xz") {
            return XzFormat;
        }
        if (ext == "tar") {
            return TarFormat;
        }
        if (ext == "xar") {
            return XarFormat;
        }
        if (ext == "zip") {
            return ZipFormat;
        }
        if (ext == "7z") {
            return SevenZipFormat;
        }
        if (ext == "zstd") {
            return ZstdFormat;
        }
        return formats(0);
    }();

    return m_ArchiveFormat != 0;
}

// Confirms all the files that are stagged for compression , Returns true
// on success and vice-versa.
// This populates m_ConfirmedFiles vector with all the files added ,
// Directory's files will be recursively added.
bool CompressorPrivate::confirmFiles() {
    qDeleteAll(m_ConfirmedFiles);
    m_ConfirmedFiles.clear();
    for(const auto& node : m_StaggedFiles) {
        short eCode = node->open();
        if(eCode != NoError) {
            emit error(eCode, !node->isInMemory ? node->path : "");
            return false;
        }

        if(!node->valid) {
            continue;
        }

        if(!node->isInMemory) {
            QFileInfo info(/* file path given by the user = */node->path);

            // Check if the file exists.
            if(!info.exists()) {
                emit error(FileDoesNotExist, info.filePath());
                return false;
            }

            // Check permission to read.
            if(!info.isReadable()) {
                emit error(NoPermissionToReadFile, info.filePath());
                return false;
            }


            // Check if it is file or a directory ,
            // if directory then add files in directory
            // recursively.
            if(info.isDir()) {
                std::deque<QString> dirList;
                QString toReplace = info.filePath();
                dirList.push_back(info.filePath());

                while (!dirList.empty()) {
                    QDir dir(dirList.front());
                    dirList.pop_front();
                    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
                    for (const auto &i : list) {
                        if(i.isDir()) {
                            dirList.push_back(i.filePath());
                            continue;
                        }
                        QString file = i.filePath();
                        auto fileNode = new Node;
                        fileNode->isInMemory = node->isInMemory;
                        fileNode->valid = node->valid;
                        fileNode->path = file;

                        if (toReplace.back() == '/' || toReplace.back() == '\\') {
                            if (node->entry.back() != '/' && node->entry.back() != '\\') {
                                node->entry.push_back('/');
                            }
                        }
                        fileNode->entry = file.replace(toReplace, node->entry);

                        m_ConfirmedFiles.push_back(fileNode);

                        n_BytesTotal += QFileInfo(i.filePath()).size();
                    }
                }
            } else { // Add it to the confirmed list.
                auto fileNode = new Node;
                fileNode->isInMemory = node->isInMemory;
                fileNode->valid = node->valid;
                fileNode->path = info.filePath();
                fileNode->entry = node->entry;
                m_ConfirmedFiles.push_back(fileNode);
                n_BytesTotal += info.size();
            }
        } else { // If QIODevice given
            auto fileNode = new Node;
            fileNode->isInMemory = node->isInMemory;
            fileNode->valid = node->valid;
            fileNode->io = node->io;
            fileNode->entry = node->entry;
            m_ConfirmedFiles.push_back(fileNode);
            n_BytesTotal += node->io->size();
        }
    }

    // TODO: If we need the ability to compress empty files,
    // then n_BytesTotal should be valid even if it is zero,
    // know why it is important to keep it a non-zero value,
    // other than the fact that if bytes total is zero, anyone
    // who depends on the progress signal to provide non-zero
    // value will segfault.
    // Let's now check if we atleast have some files to actually
    // compress.
    return !m_ConfirmedFiles.empty();
}

// Does the compression and also resumes it if called twice.
short CompressorPrivate::compress() {
    if (!m_ArchiveWrite) {
        /// Open Temporary file for write.
        if(!b_MemoryMode && !m_TemporaryFile->open(QIODevice::WriteOnly)) {
            emit error(ArchiveWriteOpenError, m_TemporaryFile->fileName());
            return ArchiveWriteOpenError;
        }

        m_ArchiveWrite = QSharedPointer<struct archive>(
                             archive_write_new(), ArchiveWriteDestructor);
        if (!m_ArchiveWrite) {
            emit error(NotEnoughMemory, !b_MemoryMode ? m_TemporaryFile->fileName() : "");
            return NotEnoughMemory;
        }

        switch (m_ArchiveFormat) {
        case BZipFormat:
        case BZip2Format:
            archive_write_add_filter_bzip2(m_ArchiveWrite.data());
            archive_write_set_format_gnutar(m_ArchiveWrite.data());
            break;
        case GZipFormat:
            archive_write_add_filter_gzip(m_ArchiveWrite.data());
            archive_write_set_format_gnutar(m_ArchiveWrite.data());
            break;
        case XzFormat:
            archive_write_add_filter_xz(m_ArchiveWrite.data());
            archive_write_set_format_gnutar(m_ArchiveWrite.data());
            break;
        case TarFormat:
            archive_write_add_filter_none(m_ArchiveWrite.data());
            archive_write_set_format_gnutar(m_ArchiveWrite.data());
            break;
        case XarFormat:
            archive_write_add_filter_none(m_ArchiveWrite.data());
            archive_write_set_format_xar(m_ArchiveWrite.data());
            break;
        case SevenZipFormat:
            archive_write_add_filter_none(m_ArchiveWrite.data());
            archive_write_set_format_7zip(m_ArchiveWrite.data());
            break;
        case ZstdFormat:
#if ARCHIVE_VERSION_NUMBER >= 3003003
            archive_write_add_filter_zstd(m_ArchiveWrite.data());
            /*
             * TODO: Investigate more on this.
             *
             * For some reason, for in-memory compression and extraction
             * ISO9660 with ZSTD filter is the only thing that works.
             * It seems to be an issue with libarchive itself.
            */
            if(!b_MemoryMode) {
                archive_write_set_format_gnutar(m_ArchiveWrite.data());
            } else {
                archive_write_set_format_iso9660(m_ArchiveWrite.data());
            }
#else // Explicitly fall-back to zip if libarchive doesn't support zstd.
            archive_write_add_filter_none(m_ArchiveWrite.data());
            archive_write_set_format_zip(m_ArchiveWrite.data());
#endif
            break;
        default:
            archive_write_add_filter_none(m_ArchiveWrite.data());
            archive_write_set_format_zip(m_ArchiveWrite.data());
            break;
        }

        // Set Password if the format is Zip and a password is given by the user.
        //
        // Note:
        // Currently only Zip format is officially supported by libarchive for the
        // ability to use passwords and thus until the user uses Zip format , the
        // password even if given is ignored.
#if ARCHIVE_VERSION_NUMBER >= 3003003
        if (!m_Password.isEmpty() && m_ArchiveFormat == ZipFormat) {
            archive_write_set_passphrase(m_ArchiveWrite.data(), m_Password.toUtf8().constData());
            archive_write_set_options(m_ArchiveWrite.data(), "zip:encryption=traditional");
        }
#endif

        if(n_BlockSize) {
            archive_write_set_bytes_per_block(m_ArchiveWrite.data(), n_BlockSize);
        }

        if(!b_MemoryMode) {
            // Finally open the write archive using the handle of the Temporary file.
            if(archive_write_open_fd(m_ArchiveWrite.data(),
                                     m_TemporaryFile->handle()) != ARCHIVE_OK) {
                m_ArchiveWrite.clear();
                emit error(ArchiveWriteOpenError, m_TemporaryFile->fileName());
                return ArchiveWriteOpenError;
            }
        } else {
            if (archiveWriteOpenQIODevice(m_ArchiveWrite.data(),
                                          m_Buffer.get()) != ARCHIVE_OK) {
                m_ArchiveWrite.clear();
                emit error(ArchiveWriteOpenError, {});
                return ArchiveWriteOpenError;
            }
        }

        n_TotalEntries = m_ConfirmedFiles.size(); // for reporting progress.
    }

    // Start compressing files.
    while (!m_ConfirmedFiles.empty()) {
        auto node = m_ConfirmedFiles.front();
        int r;
        std::size_t len;
        char buff[16384];

        if(!node->isInMemory) {
            // TODO:
            //	Implement a failsafe copy mechanism.
            //
            // Note:
            // Down below implementation is nearly good but
            // not very robust and thus needs a good implementation.
            QSharedPointer<struct archive> disk(
                archive_read_disk_new(), ArchiveReadDestructor);
            archive_read_disk_set_standard_lookup(disk.data());

            r = archive_read_disk_open(disk.data(),
                                       QFile::encodeName(/*file path = */node->path).constData());

            if(r != ARCHIVE_OK) {
                emit error(DiskOpenError, node->path);
                return DiskOpenError;
            }

            for (;;) {
                QSharedPointer<struct archive_entry> entry(
                    archive_entry_new(), ArchiveEntryDestructor);
                r = archive_read_next_header2(disk.data(), entry.data());

                if (r == ARCHIVE_EOF) {
                    break;
                }

                if (r != ARCHIVE_OK) {
                    emit error(DiskReadError, node->path);
                    return DiskReadError;
                }

                archive_read_disk_descend(disk.data());
                archive_entry_set_pathname(entry.data(), (node->entry).toUtf8().constData());
                r = archive_write_header(m_ArchiveWrite.data(), entry.data());

                if (r == ARCHIVE_FATAL) {
                    emit error(ArchiveFatalError, node->path);
                    return ArchiveFatalError;
                }
                if (r > ARCHIVE_FAILED) {
                    QFile file(node->path);
                    if(!file.open(QIODevice::ReadOnly)) {
                        emit error(DiskOpenError, node->path);
                        return DiskOpenError;
                    }
                    len = file.read(buff, sizeof(buff));
                    while (len > 0) {
                        archive_write_data(m_ArchiveWrite.data(), buff, len);
                        n_BytesProcessed += len;

                        emit progress(node->entry,
                                      (n_TotalEntries - (m_ConfirmedFiles.size() - 1)),
                                      n_TotalEntries, n_BytesProcessed, n_BytesTotal);

                        QCoreApplication::processEvents();
                        len = file.read(buff, sizeof(buff));
                    }
                    file.close();
                } else {
                    emit error(ArchiveHeaderWriteError, node->path);
                    return ArchiveHeaderWriteError;
                }

                QCoreApplication::processEvents();
            }
        } else {
            QSharedPointer<struct archive_entry> entry(
                archive_entry_new(),
                ArchiveEntryDestructor);

            // Setup archive entry.
#if (QT_VERSION >= QT_VERSION_CHECK(5, 8, 0))
            auto datetime = static_cast<time_t>(QDateTime::currentDateTime().toSecsSinceEpoch());
#else
            auto datetime = static_cast<time_t>(QDateTime::currentDateTime().toTime_t());
#endif

            archive_entry_set_pathname(entry.data(), (node->entry).toUtf8().constData());
            archive_entry_set_filetype(entry.data(), AE_IFREG);
            archive_entry_set_size(entry.data(), (node->io)->size());
            archive_entry_set_atime(entry.data(), datetime, 0);
            archive_entry_set_mtime(entry.data(), datetime, 0);
            archive_entry_set_birthtime(entry.data(), datetime, 0);

            (node->io)->seek(0);

            // Write entry to memory
            r = archive_write_header(m_ArchiveWrite.data(), entry.data());

            if (r == ARCHIVE_FATAL) {
                emit error(ArchiveFatalError, node->entry);
                return ArchiveFatalError;
            }

            if (r <= ARCHIVE_FAILED) {
                emit error(ArchiveHeaderWriteError, node->entry);
                return ArchiveHeaderWriteError;
            }
            len = (node->io)->read(buff, sizeof(buff));
            while (len > 0) {
                archive_write_data(m_ArchiveWrite.data(), buff, len);
                n_BytesProcessed += len;

                emit progress(node->entry,
                    (n_TotalEntries - (m_ConfirmedFiles.size() - 1)),
                    n_TotalEntries, n_BytesProcessed, n_BytesTotal);

                QCoreApplication::processEvents();
                len = (node->io)->read(buff, sizeof(buff));
            }
        }

        m_ConfirmedFiles.pop_front();

        emit progress(node->entry,
                      (n_TotalEntries - m_ConfirmedFiles.size()),
                      n_TotalEntries, n_BytesProcessed, n_BytesTotal);

        delete node;

        QCoreApplication::processEvents();
        if(b_PauseRequested) {
            b_PauseRequested = false;
            return OperationPaused;
        }

        if(b_CancelRequested) {
            b_CancelRequested = false;
            m_ConfirmedFiles.clear();
            m_ArchiveWrite.clear();
            return OperationCanceled;
        }
    }
    m_ArchiveWrite.clear();
    return NoError;
}
