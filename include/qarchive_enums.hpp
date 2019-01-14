#ifndef QARCHIVE_ENUMS_HPP_INCLUDED
#define QARCHIVE_ENUMS_HPP_INCLUDED
namespace QArchive
{

/*
 * Common error codes , these are most likely will be
 * used across all QArchive Objects , Thus these are
 * given an global error code values.
*/
enum : short {
    NoError = 0,
    OperationCanceled,
    NotEnoughMemory
};

/*
 * DiskExtractor error codes , these error codes are
 * only valid for the DiskExtractor class.
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
    NoPermissionToWrite,
    InvalidOutputDirectory,
    InvalidArchiveFile,
};
}
#endif
