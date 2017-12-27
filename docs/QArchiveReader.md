---
id: QArchiveReader
title: Class QArchive::Reader
sidebar_label: QArchive::Reader
---

### Public Functions

|                                   |                                                                                   |
|-----------------------------------|-----------------------------------------------------------------------------------|
| **explicit**                      | Reader(QObject *parent = NULL)                                                    |
| **explicit**                      | Reader(const QString& archive)                                                    |
| **void**                          | setArchive(const QString& archive)                                                |
| **const QStringList&**            | listFiles(void)                                                                   |
| **void**                          | clear(void)                                                                       |

### Slots

All slots used by you are **inherited** from **[QThread](http://doc.qt.io/qt-5/qthread.html)**

|               |             |
|---------------|-------------|
| **void**  	| start(void) |
| **void**      | quit(void)  |
| **void**      | wait(void)  |

### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**            | archiveFiles(const QString& archive, const QStringList& files)                  |
| **void**  	      | error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& what)	    |   
