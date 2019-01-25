---
id: QArchiveErrorCodes
title: QArchive Error Codes
sidebar_label: QArchive Error Codes
---

These error codes will be emitted by a common signal present in compressor and extractor classes
 which follows a prototype like this ``` error(short errorCode , QString file) ```. 
The **file** can give a clue on what file is this error belongs to.

You can also convert error code to a QString which is the variable name for that error
code using the static member method ```errorCodeToString``` present in all compressor 
and extractor classes.


| Variable Name                       	  | Value   |
|-----------------------------------------|---------|
| QArchive::NoError			  |    0    |
| QArchive::NotEnoughMemory		  |    1    |
| QArchive::NoPermissionToWrite		  |    2    |
| QArchive::ArchiveNotGiven               |    3    |
| QArchive::ArchiveDoesNotExists          |    4    |
| QArchive::ArchiveReadError              |    5    | 
| QArchive::ArchiveWriteError		  |    6    |
| QArchive::ArchiveHeaderWriteError       |    7    |
| QArchive::ArchiveCorrupted              |    8    |
| QArchive::ArchiveIsNotReadable          |    9    |
| QArchive::ArchiveIsNotOpened            |    10   |
| QArchive::ArchivePasswordNeeded         |    11   |
| QArchive::ArchivePasswordIncorrect      |    12   |
| QArchive::CannotOpenArchive             |    13   |
| QArchive::NoPermissionToReadArchive     |    14   |
| QArchive::InvalidOutputDirectory        |    15   |
| QArchive::InvalidArchiveFile            |    16   |
| QArchive::ArchiveFileNameNotGiven       |    25   |
| QArchive::ArchiveFileAlreadyExists      |    26   |
| QArchive::ArchiveWriteOpenError         |    27   |
| QArchive::ArchiveFatalError             |    28   |
| QArchive::FileDoesNotExist		  |    29   |
| QArchive::DiskReadError                 |    30   |
| QArchive::DiskOpenError                 |    31   |
| QArchive::NoPermissionToReadFile        |    32   |
| QArchive::NoFilesToCompress             |    33   |


