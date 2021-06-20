#ifndef QARCHIVE_ENUMS_HPP_INCLUDED
#define QARCHIVE_ENUMS_HPP_INCLUDED
class QString;
namespace QArchive {
/* A function that converts the enum value to its variable
 * name to be used by users.
*/
QString errorCodeToString(short);

/*
 * Common error codes, these are most likely will be
 * used across all QArchive Objects, Thus these are
 * given an global error code values.
*/
enum : short {
    NoError = 0,
    NotEnoughMemory,
    NoPermissionToWrite
};

/*
 * Extractor error codes, these error codes are
 * only valid for the Extractor class.
 * The class is responsible for extraction of data from
 * Archives.
*/
enum : short {
    ArchiveNotGiven = 3,
    ArchiveDoesNotExists,
    ArchiveReadError,
    ArchiveWriteError,
    ArchiveHeaderWriteError,
    ArchiveCorrupted,
    ArchiveIsNotReadable,
    ArchiveIsNotOpened,
    ArchivePasswordNeeded,
    ArchivePasswordIncorrect,
    CannotOpenArchive,
    NoPermissionToReadArchive,
    InvalidOutputDirectory,
    InvalidArchiveFile,
};

/*
 * Compressor error codes.
*/
enum : short {
    ArchiveFileNameNotGiven = 100,
    ArchiveFileAlreadyExists,
    ArchiveWriteOpenError,
    ArchiveFatalError,
    FileDoesNotExist,
    DiskReadError,
    DiskOpenError,
    NoPermissionToReadFile,
    NoFilesToCompress,
    CannotOpenFile,
    IODeviceSequential
};

/*
 * Compressor Supported Formats.
 * These codes are used to represent Archive formats in form
 * of a short integer.
*/
enum : short {
    TarFormat = 200,
    ZipFormat,
    SevenZipFormat,
    BZipFormat,
    BZip2Format,
    GZipFormat,
    XzFormat,
    XarFormat
};
}
#endif
