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

|               |             |
|---------------|-------------|
| **void**  	| start(void) |
| **void**      | stop(void)  |

### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**	      | stopped(void)									|
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

Start reading all files in the archive.

#### void stop(void)
This member function is a **[SLOT]**

Stops the Reader. **void stopped(void)** signal is emitted when this is successfull.

#### void archiveFiles(const QString& archive, const QStringList& files)
This member function is a **[SIGNAL]**

Emitted when the Reader successfully **reads** the files inside the archive.

#### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& what)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with the reader. refer the [error codes](QArchiveErrorCodes.md)
