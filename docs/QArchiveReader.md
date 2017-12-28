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

## Member Functions Documentation 

#### explicit Reader(QObject *parent = NULL)

Constructs **Reader** and sets the **QObject parent** as **0**

#### explicit Reader(const QString& archive)

Points to an single archive given by the user.

#### const QStringList& listFiles(void)

Returns all files added to the archive which will be created!

#### void clear(void)

Clears all cached **filenames** and **archive paths**

#### void start(void)
This member function is a **[SLOT]**

Reads all files in the archive. Inherited from **[QThread](http://doc.qt.io/qt-5/qthread.html)**.

#### void quit(void)
This member function is a **[SLOT]**

Quits the Reader thread. Inherited from **[QThread](http://doc.qt.io/qt-5/qthread.html)**

#### void archiveFiles(const QString& archive, const QStringList& files)
This member function is a **[SIGNAL]**

Emitted when the Reader successfully **reads** the files inside the archive.

#### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& what)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with the reader. refer the [error codes](QArchiveErrorCodes.md)
