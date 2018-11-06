#ifndef QARCHIVE_ENUMS_HPP_INCLUDED
#define QARCHIVE_ENUMS_HPP_INCLUDED
namespace QArchive
{
/* Error Codes. */

// common error codes.
enum : short {
    NoError = 0,
    OperationCanceled,
    NotEnoughMemory
};

// disk extractor error codes.
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
    InvalidQFile,
};
}
#endif
