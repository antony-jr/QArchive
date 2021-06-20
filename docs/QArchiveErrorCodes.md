---
id: QArchiveErrorCodes
title: QArchive Error Codes
sidebar_label: QArchive Error Codes
---

You convert error code to a QString which is the variable name for that error 
code using the function   ```QArchive::errorCodeToString(short)```.


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
| QArchive::ArchiveFileNameNotGiven       |   100   |
| QArchive::ArchiveFileAlreadyExists      |   101   |
| QArchive::ArchiveWriteOpenError         |   102   |
| QArchive::ArchiveFatalError             |   103   |
| QArchive::FileDoesNotExist		  |   104   |
| QArchive::DiskReadError                 |   105   |
| QArchive::DiskOpenError                 |   106   |
| QArchive::NoPermissionToReadFile        |   107   |
| QArchive::NoFilesToCompress             |   108   |
| QArchive::CannotOpenFile		  |   109   |
| QArchive::IODeviceSequential		  |   110   |
