#include <QString>
#include <qarchive_enums.hpp>

using namespace QArchive;

QString QArchive::errorCodeToString(short errorCode) {
    QString errorString;
    switch(errorCode) {
    case NoError:
        errorString = QString("QArchive::NoError");
        break;
    case NotEnoughMemory:
        errorString = QString("QArchive::NotEnoughMemory");
        break;
    case NoPermissionToWrite:
        errorString = QString("QArchive::NoPermissionToWrite");
        break;
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
    case ArchiveFileNameNotGiven:
        errorString = QString("QArchive::ArchiveFileNameNotGiven");
        break;
    case ArchiveFileAlreadyExists:
        errorString = QString("QArchive::ArchiveFileAlreadyExists");
        break;
    case ArchiveWriteOpenError:
        errorString = QString("QArchive::ArchiveWriteOpenError");
        break;
    case ArchiveFatalError:
        errorString = QString("QArchive::ArchiveFatalError");
        break;
    case FileDoesNotExist:
        errorString = QString("QArchive::FileDoesNotExist");
        break;
    case DiskReadError:
        errorString = QString("QArchive::DiskReadError");
        break;
    case DiskOpenError:
        errorString = QString("QArchive::DiskOpenError");
        break;
    case NoPermissionToReadFile:
        errorString = QString("QArchive::NoPermissionToReadFile");
        break;
    case NoFilesToCompress:
        errorString = QString("QArchive::NoFilesToCompress");
        break;
    case CannotOpenFile:
	errorString = QString("QArchive::CannotOpenFile");
	break;
    case IODeviceSequential:
	errorString = QString("QArchive::IODeviceSequential");
	break;
    default:
        errorString = QString("QArchive::UnknownError");
        break;
    }
    return errorString;
}
