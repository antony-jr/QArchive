---
id: QArchiveDiskCompressor
title: Class QArchive::DiskCompressor
sidebar_label: QArchive::DiskCompressor
---

The QArchive::DiskCompressor class helps to create archives that are supported by **libarchive**.

**Note:** All functions in this class is **[reentrant](https://doc.qt.io/qt-5/threads-reentrancy.html)**.


### Public Functions

|                                                                                                        |
|--------------------------------------------------------------------------------------------------------|
| DiskCompressor(QObject ```*```parent = nullptr, bool singleThreaded = true)                            | 
| DiskCompressor(const QString&, QObject ```*```parent = nullptr,  bool singleThreaded = true)           |
| DiskCompressor(const QString&, short, QObject ```*```parent = nullptr, bool singleThreaded = true)     |


### Slots

|                   |                                                                                                |
|-------------------|------------------------------------------------------------------------------------------------|


### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**	      | [started](#void-startedvoid)()	              			                |
| **void**            | [finished](#void-finishedvoid)()                        	                |
| **void**            | [paused](#void-pausedvoid)()                                                    |
| **void**            | [resumed](#void-resumedvoid)()                                                  |
| **void**            | [canceled](#void-canceledvoid)()                                                |
| **void**            | [progress](#void-progressint-percentage)()                                      |
| **void**  	      | [error](#errorshort-errorcode-qarchive-docs-qarchiveerrorcodeshtml-const-qstring-file)(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)|


## Member Functions Documentation


### void addFiles(const QString &file)

Adds **a single file** as the given **QString**.

> Please See that if you add a file directly like 'MyFiles/Ouput.txt' then it will be added
with the same parent directory.

### void addFiles(const QStringList &files)

Adds **a list of file(s)** as the given **QStringList**.


### void removeFiles(const QString &file)

Removes a **single file** from the archive.

### void removeFiles(const QStringList &files)

Removes a **a list of file(s)** from the archive.

### void clear(void)

Clears the internal cache.

```
 Compressor.clear();
```

### void start(void)
This member function is a **[SLOT]**

Starts the compression of the file(s).

### void pause(void)
This member function is a **[SLOT]**

Pauses the compression of the file(s). This slot is **async** and thus you need to wait for the
**paused** signal , Which confirms that the pause call was successfull.

### void resume(void)
This member function is a **[SLOT]**

Resumes the compression of the file(s). This slot is **async** and thus you need to wait for the
**resumed** signal , Which confirms that the resume call was successfull.

### void cancel(void)
This member function is a **[SLOT]**

Cancels the compression of the file(s). This slot is **async** and thus you need to wait for the
**canceled** signal , Which confirms that the cancel call was successfull.
 
### void started(void)
This member function is a **[SIGNAL]**

Emitted when the compression is started.

### void finished(void)
This member function is a **[SIGNAL]** 

Emitted when the compression is finished successfully.

### void paused(void)
This member function is a **[SIGNAL]** 

Emitted when the compression is paused successfully.

### void resumed(void)
This member function is a **[SIGNAL]** 

Emitted when the compression is resumed successfully.

### void canceled(void)
This member function is a **[SIGNAL]** 

Emitted when the compression is canceled successfully.

### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with an archive. refer the [error codes](QArchiveErrorCodes.md).
