---
id: QArchiveErrorCodes
title: QArchive error codes
sidebar_label: QArchive error codes
---

| Error Code                       	| Meaning                                                                 	|
|----------------------------------	|-------------------------------------------------------------------------	|
| QArchive::NO_ARCHIVE_ERROR       	| No Problem in the archive , successfully extracted.                     	|
| QArchive::ARCHIVE_QUALITY_ERROR  	| Corrupted archive , cannot extract.                                     	|
| QArchive::ARCHIVE_READ_ERROR     	| Cannot find the archive in the mentioned path or some other read error. 	|
| QArchive::ARCHIVE_UNCAUGHT_ERROR 	| Unknown error.                                                          	|   
| QArchive::INVALID_DEST_PATH       | The Destination directory does not exist or some write error.             |
| QArchive::DISK_OPEN_ERROR         | Failed to read a file that you want to compress.                          |
| QArchive::DISK_READ_ERROR         | Corrupt file in the list of files you wanted to compress.                 |
| QArchive::FILE_NOT_EXIST          | The file you mentioned in the compression list is not there.              |
