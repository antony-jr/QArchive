---
id: QArchiveCompressor
title: Class QArchive::Compressor
sidebar_label: QArchive::Compressor
---

### Public Functions

|                                   |                                                                                   |
|-----------------------------------|-----------------------------------------------------------------------------------|
| **explicit**                      | Compressor(QObject *parent = NULL)                                                |
| **explicit**                      | Compressor(const QString& archive , const QStringList& files)                     |
| **explicit**                      | Compressor(const QString& archive , const QString& file)                          |
| **explicit**                      | Compressor(const QString& archive)                                                |
| **void**                          | addFiles(const QString& file)                                                     |
| **void**                          | addFiles(const QStringList& files)                                                |
| **void**                          | removeFiles(const QStringList& files)                                             |
| **void**                          | setArchive(const QString& archive)                                                |
| **void**                          | setArchiveFormat(short **[ArchiveFormat](QArchiveFormats.md)** )                  |

### Slots

|               |             |
|---------------|-------------|
| **void**  	| start(void) |
| **void**      | stop(void)  |

### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**	      | stopped(void)								        |
| **void**            | finished(void)                        	                                        |
| **void**    	      | compressing(const QString& file)                                                |
| **void**    	      | compressed(const QString& file)                                       	        |
| **void**  	      | error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)	    |  

## Member Functions Documentation

#### explicit Compressor(QObject *parent = NULL)

Constructs **Compressor** and sets the **QObject parent**.

#### explicit Compressor(const QString& archive , const QStringList& files)

Constructs **Compressor** and sets the archive path which will be created and adds a set files to the archive.

#### explicit Compressor(const QString& archive , const QString& file) 

This is a overloaded function. This sets the archive path which will be created and only adds a single file or folder.

#### void addFiles(const QString& file)

This adds a file to the archive.

#### void addFiles(const QStringList& files)

This is an overloaded function. Adds a list of files to the archive.

#### void removeFiles(const QStringList& files)

Removes a set of files from the archive.

#### void setArchive(const QString& archive)

Sets the destination of the archive that is beign created.

#### void setArchiveFormat(short **[ArchiveFormat](QArchiveFormats.md)**)

Sets the format of the archive which is going to be created. refer the **[supported formats](QArchiveFormats.md)**

#### void start(void)
This member function is a **[SLOT]**

Starts the compression of all files in the archive.

#### void stop(void)
This member function is a **[SLOT]**

Stops the compressor. **void stopped(void)** signal will be emitted if this slot is successfull.

#### void stopped(void)
This member function is a **[SIGNAL]**

Emitted when **void stop(void)** is successfull.

#### void finished(void)
This member function is a **[SIGNAL]** 

Emitted when all jobs are done. (i.e) When all compression is finished.

#### void compressing(const QString& file)
This member function is a **[SIGNAL]**

Emitted when an file is started compressing from the archive.

#### void compressed(const QString& file)
This member function is a **[SIGNAL]**

Emitted when a file is compressed successfully!

#### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with a compression. refer the [error codes](QArchiveErrorCodes.md)


