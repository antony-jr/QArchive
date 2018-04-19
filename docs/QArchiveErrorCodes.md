---
id: QArchiveErrorCodes
title: QArchive error codes
sidebar_label: QArchive error codes
---

These error codes will be emitted by a common signal which follows a prototype like this  
``` error(short errorCode , QString errorMessage) ```.   
The **errorMessage** can give a clue on what file is this error belongs to.  



| Error Code                       	  | Meaning                                       |  Value  |
|-------------------------------------|-----------------------------------------------|---------|
| QArchive::NO_ARCHIVE_ERROR       	  | No error occured.                          	  |    0    |
| QArchive::ARCHIVE_QUALITY_ERROR  	  | Archive has bad data.                         |    1    |
| QArchive::ARCHIVE_READ_ERROR        | Archive does not exist or no permission.      |    2    |
| QArchive::ARCHIVE_UNCAUGHT_ERROR    | Unknown error.                                |    3    |
| QArchive::ARCHIVE_FATAL_ERROR       | Fatal error.                                  |    4    |
| QArchive::ARCHIVE_WRONG_PASSWORD    | Wrong password was given for the archive.     |    5    |
| QArchive::ARCHIVE_PASSWORD_NOT_GIVEN| Empty password was given for the archive.     |    6    |
| QArchive::ARCHIVE_WRITE_OPEN_ERROR  | Cannot create archive.                        |    7    |
| QArchive::DISK_OPEN_ERROR           | Cannot open the added file.                   |    8    |
| QArchive::DISK_READ_ERROR           | Cannot read the added file.                   |    9    |
| QArchive::DESTINATION_NOT_FOUND     | Cannot find the given extraction destination. |    10   |
| QArchive::FILE_NOT_EXIST            | The given file does not exists for sure.      |    11   |
| QArchive::INVALID_DEST_PATH         | Given extraction destination is invalid.      |    12   |
| QArchive::NOT_ENOUGH_MEMORY         | Allocation error.                             |    13   |
| QArchive::FILE_OPEN_FAILED          | Cannot open the given file but it exists.     |    14   |
