#include <QString>
#include <qarchive_enums.hpp>

using namespace QArchive;

QString QArchive::errorCodeToString(short errorCode) {
    switch(errorCode) {
    case NoError:
        return "QArchive::NoError";
    case NotEnoughMemory:
        return "QArchive::NotEnoughMemory";
    case NoPermissionToWrite:
        return "QArchive::NoPermissionToWrite";
    case ArchiveNotGiven:
        return "QArchive::ArchiveNotGiven";
    case ArchiveDoesNotExists:
        return "QArchive::ArchiveDoesNotExists";
    case ArchiveReadError:
        return "QArchive::ArchiveReadError";
    case ArchiveWriteError:
        return "QArchive::ArchiveWriteError";
    case ArchiveHeaderWriteError:
        return "QArchive::ArchiveHeaderWriteError";
    case ArchiveCorrupted:
        return "QArchive::ArchiveCorrupted";
    case ArchiveIsNotReadable:
        return "QArchive::ArchiveIsNotReadable";
    case ArchiveIsNotOpened:
        return "QArchive::ArchiveIsNotOpened";
    case ArchivePasswordNeeded:
        return "QArchive::ArchivePasswordNeeded";
    case ArchivePasswordIncorrect:
        return "QArchive::ArchivePasswordIncorrect";
    case CannotOpenArchive:
        return "QArchive::CannotOpenArchive";
    case NoPermissionToReadArchive:
        return "QArchive::NoPermissionToReadArchive";
    case InvalidOutputDirectory:
        return "QArchive::InvalidOutputDirectory";
    case InvalidArchiveFile:
        return "QArchive::InvalidArchiveFile";
    case ArchiveFileNameNotGiven:
        return "QArchive::ArchiveFileNameNotGiven";
    case ArchiveFileAlreadyExists:
        return "QArchive::ArchiveFileAlreadyExists";
    case ArchiveWriteOpenError:
        return "QArchive::ArchiveWriteOpenError";
    case ArchiveFatalError:
        return "QArchive::ArchiveFatalError";
    case FileDoesNotExist:
        return "QArchive::FileDoesNotExist";
    case DiskReadError:
        return "QArchive::DiskReadError";
    case DiskOpenError:
        return "QArchive::DiskOpenError";
    case NoPermissionToReadFile:
        return "QArchive::NoPermissionToReadFile";
    case NoFilesToCompress:
        return "QArchive::NoFilesToCompress";
    case CannotOpenFile:
        return "QArchive::CannotOpenFile";
    case IODeviceSequential:
        return "QArchive::IODeviceSequential";
    default:
        return "QArchive::UnknownError";
    }
    return "";
}
