---
id: QArchiveDiskCompressor
title: Class QArchive::DiskCompressor
sidebar_label: QArchive::DiskCompressor
---

The QArchive::DiskCompressor class helps to create archives that based on hard drive, (i.e) The storage of 
the computer. This compresses files in archive formats supported by **libarchive**.


|	    |				               |		
|-----------|------------------------------------------|
|  Header:  | #include < QArchive/QArchive >	       |
|   qmake:  | include(QArchive/QArchive.pri)           |
| Class Name| DiskCompressor                           |
| Namespace | QArchive				       |
| Inherits: | QObject			               |

This class belongs to QArchive namespace, so make sure to include it.   
**Note:** All functions in this class is **[reentrant](https://doc.qt.io/qt-5/threads-reentrancy.html)**.


### Public Functions

|                                                                                                        |
|--------------------------------------------------------------------------------------------------------|
| [DiskCompressor](#diskcompressorqobject-parent--nullptr--bool-singlethreaded--true)(QObject \*parent = nullptr, bool singleThreaded = true)| 
| [DiskCompressor](#diskcompressorconst-qstring-archivepath--qobject-parent--nullptr--bool-singlethreaded--true)(const QString&, QObject \*parent = nullptr,  bool singleThreaded = true)|
| [DiskCompressor](#diskcompressorconst-qstring-archivepath--short-archiveformat--qobject-parent--nullptr--bool-singlethreaded--true)(const QString&, short, QObject \*parent = nullptr, bool singleThreaded = true)|


### Slots

|           |                                                                                                |
|-----------|------------------------------------------------------------------------------------------------|
| **void**  | [setFileName](#void-setfilenameconst-qstring-archivepath)(const QString&)		             |
| **void**  | [setArchiveFormat](#void-setarchiveformatshort-archiveformat)(short)	                     |
| **void**  | [setPassword](#void-setpasswordconst-qstring-password)(const QString&)                         |
| **void**  | [setBlockSize](#void-setblocksizeint-size)(int)						     |
| **void**  | [addFiles](#void-addfilesconst-qstring-file)(const QString&)                                   |
| **void**  | [addFiles](#void-addfilesconst-qstringlist-files)(const QStringList&)                          |
| **void**  | [addFiles](#void-addfilesconst-qstring-entryname-const-qstring-file)(const QString&, const QString&)|
| **void**  | [addFiles](#void-addfilesconst-qstringlist-entryname-const-qstringlist-file)(const QStringList&, const QStringList&)|
| **void**  | [removeFiles](#void-removefilesconst-qstring-file)(const QString&)                                   |
| **void**  | [removeFiles](#void-removefilesconst-qstringlist-files)(const QStringList&)                          |
| **void**  | [removeFiles](#void-removefilesconst-qstring-entryname--const-qstring-file)(const QString&, const QString&)|
| **void**  | [removeFiles](#void-removefilesconst-qstringlist-entryname--const-qstringlist-file)(const QStringList&, const QStringList&)|
| **void**  | [clear](#void-clearvoid)(void)                                                                 |
| **void**  | [start](#void-startvoid)(void)                                                                 |
| **void**  | [pause](#void-pausevoid)(void)                                                                 |
| **void**  | [resume](#void-resumevoid)(void)                                                               |
| **void**  | [cancel](#void-cancelvoid)(void)                                                               |



### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**            | [progress](#void-progressqstring-file--int-processedentries--int-totalentries--qint64-bytesprocessed-qint64-bytestotal)(QString, int, int, qint64, qint64)|
| **void**	      | [started](#void-startedvoid)(void)                                              |
| **void**            | [finished](#void-finishedvoid)(void)                        	                |
| **void**            | [paused](#void-pausedvoid)(void)                                                |
| **void**            | [resumed](#void-resumedvoid)(void)                                              |
| **void**            | [canceled](#void-canceledvoid)(void)                                            |
| **void**  	      | [error](#errorshort-errorcodeqarchivedocsqarchiveerrorcodeshtml--const-qstring-file)(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)|




## Member Functions Documentation


### DiskCompressor(QObject \*parent = nullptr , bool singleThreaded = true)

Constructs **DiskCompressor** and sets the given QObject as the parent.
By default the **DiskCompressor** is constructed without any parent and runs in a single thread.
The **DiskCompressor** can be made to run in a different thread if ```singleThreaded``` is set
to false.

```
 QCoreApplication app(argc , argv);
 
 // Default construction , no parent and single threaded
 QArchive::DiskCompressor Compressor;

 // With parent and single threaded
 auto PCompressor = new QArchive::DiskCompressor(&app);
 
 // With parent and non-single threaded
 auto PDCompressor = new QArchive::DiskCompressor(&app , false);
```

---

### DiskCompressor(const QString &archivePath , QObject \*parent = nullptr , bool singleThreaded = true)

This is an overloaded constructor. This expects a QString which is assumed to be the file path of
the archive which is to be written. In this case , The format is guessed using the given archive path.
One can also explicitly set the format later using **setArchiveFormat** method.

```
 QCoreApplication app(argc , argv);
 QString archivePath("/tmp/SomeArchive.zip");
 
 // Default construction , no parent and single threaded
 QArchive::DiskCompressor Compressor(archivePath);
 
 // With parent and single threaded
 auto PCompressor = new QArchive::DiskCompressor(archivePath ,  &app);
 
 // With parent and non-single threaded
 auto PDCompressor = new QArchive::DiskCompressor(archivePath , &app , false);

```

---


### DiskCompressor(const QString &archivePath , short archiveFormat , QObject \*parent = nullptr , bool singleThreaded = true)

This is an overloaded constructor. This expects a QString which is assumed to be the file path of
the archive which is to be written and a short integer which is assumed to be the format of the 
archive to be written with respect to the [archive format codes](QArchiveFormats.md).
In this case , The format will not be guessed and uses the given archive format only regardless of the extension.

```
 QCoreApplication app(argc , argv);
 QString archivePath("/tmp/SomeArchive.data");
 
 // Default construction , no parent and single threaded
 QArchive::DiskCompressor Compressor(archivePath , QArchive::ZipFormat);
 
 // With parent and single threaded
 auto PCompressor = new QArchive::DiskCompressor(archivePath , QArchive::ZipFormat , &app);
 
 // With parent and non-single threaded
 auto PDCompressor = new QArchive::DiskCompressor(archivePath ,
						  QArchive::ZipFormat , 
						  &app , 
						  false);

```

---

### void setFileName(const QString &archivePath)
<p align="right"><code>[SLOT]</code></b></p>

Sets the path to archive which is to be written.

```
 QArchive::DiskCompressor Compressor;
 Compressor.setFileName("Test.7z");
```

---

### void setArchiveFormat(short archiveFormat)
<p align="right"><code>[SLOT]</code></b></p>

Sets the format of the archive which is to be written with respect to the 
[archive format codes](QArchiveFormats.md) , regardless of the filename extension.

```
 QArchive::DiskCompressor Compressor;
 Compressor.setFileName("Test.data");
 Compressor.setArchiveFormat(QArchive::ZipFormat);
```

---

### void setPassword(const QString &password)
<p align="right"><code>[SLOT]</code></b></p>

Sets password for the archive which is to be written. Only **Zip format** is supported for now.

```
 QArchive::DiskCompressor Compressor;
 Compressor.setFileName("Test.data");
 Compressor.setArchiveFormat(QArchive::ZipFormat);
 Compressor.setPassword("Test123"); // Ignored if format is not zip.
```

---

### void setBlockSize(int size)
<p align="right"><code>[SLOT]</code></p>

Sets the blocksize for the archive which is to be written.

---

### void addFiles(const QString &file)
<p align="right"><code>[SLOT]</code></b></p>

Adds **a single file** to the archive which is to be written. The **entry name** for this file will be assumed as **QFileInfo(file).fileName()**

---

### void addFiles(const QStringList &files)
<p align="right"><code>[SLOT]</code></b></p>

Adds **a list of file(s)** to the archive which is to be written. For each file in the list the **entry name** will be assumed as **QFileInfo(file).fileName()**. If entry name already exists in the compressor then it will be ignored silently.


---

### void addFiles(const QString &entryName , const QString &file)
<p align="right"><code>[SLOT]</code></b></p>

Adds **a single file** to the archive which is to be written under the given **entry name**.

```
 QArchive::DiskCompressor Compressor("Test.7z");
 Compressor.addFiles(/*entry name =*/"Data.txt" ,
                     /*Actual path=*/"Data/Test.txt");
 /*
  * Data/Test.txt will be compressed in the archive
  * as Data.txt without any parent since the entry
  * name also does not contain any.
  */
```

---

### void addFiles(const QStringList &entryName , const QStringList &file)
<p align="right"><code>[SLOT]</code></b></p>

Adds **a list of file(s)** to the archive which is to be written under the given corresponding  **entry name(s)**.

```
 QArchive::DiskCompressor Compressor("Test.7z");
 Compressor.addFiles(/*entry name(s)=*/ QStringList() << "Data.txt"
                                                      << "Data2.txt" ,
                     /*Actual path=*/   QStringList() << "Data/Test.txt"
                                                      << "Data/Test2.txt"                  
  );
 /*
  * "Data/Test.txt" file will be compressed as "Data.txt" and 
  * "Data/Test2.txt" file will be compressed as "Data2.txt".
  */
```

---

### void removeFiles(const QString &file)
<p align="right"><code>[SLOT]</code></b></p>

Removes **a single file** from the archive which is to be written.

> As of **v2.1.x** this file string is assumed to refer the entry name. In previous versions this file refers to the path of the file as given in addFiles()

---

### void removeFiles(const QStringList &files)
<p align="right"><code>[SLOT]</code></b></p>

Removes **a list of file(s)** from the archive which is to be written.

> As of **v2.1.x** this file list is assumed to refer the entry name. In previous versions this file refers to the path of the file as given in addFiles()

---

### void removeFiles(const QString &entryName , const QString &file)
<p align="right"><code>[SLOT]</code> <b style="color: red;">(OBSOLETE)</b></b></p>

Removes **a single file** from the archive which is to be written with respect to the **entry name**.

```
 QArchive::DiskCompressor Compressor("Test.7z");
 Compressor.addFiles(/*entry name =*/"Data.txt" ,
                     /*Actual path=*/"Data/Test.txt");

 /*
  * Has to be same as addFiles data in order to 
  * remove it.
 */
 Compressor.removeFiles(/*entry name =*/ "Data.txt",
			/*Actual path=*/ "Data/Test.txt"); 

 // As of v2.1.x the actual path is not actually respected at all. 
 // Only the entry name is used. So it's better to just use
 // removeFiles("Data.txt");
```

> As of **v2.1.x** you don't need to know the path of the file you added. Only entry name is respected. This function continues to work but it's not recommended. The given path is ignored.

### void removeFiles(const QStringList &entryName , const QStringList &file)
<p align="right"><code>[SLOT]</code> <b style="color: red;">(OBSOLETE)</b></p>

Removes **a list of file(s)** from the archive which is to be written with respect to the corresponding  **entry name(s)**.

```
 QArchive::DiskCompressor Compressor("Test.7z");
 Compressor.addFiles(/*entry name(s)=*/ QStringList() << "Data.txt"
                                                      << "Data2.txt" ,
                     /*Actual path=*/   QStringList() << "Data/Test.txt"
                                                      << "Data/Test2.txt"               
  );


  /* You have to use the exact same data to remove it. */
  Compressor.removeFiles(/*entry name(s)=*/ QStringList() << "Data.txt"
                                                          << "Data2.txt" ,
                        /*Actual path=*/   QStringList() << "Data/Test.txt"
                                                         << "Data/Test2.txt" 
  );

  // As of v2.1.x the actual path is not actually respected at all. 
  // Only the entry name is used. So it's better to just use
  // removeFiles(QStringLists() << "Data.txt" << "Data2.txt");

```

> As of **v2.1.x** you don't need to know the path of the file you added. Only entry name is respected. This function continues to work but it's not recommended. The given path is ignored.



### void clear(void)
<p align="right"><code>[SLOT]</code></p>

Clears the internal cache.

---

### void start(void)
<p align="right"><code>[SLOT]</code></p>

Starts the compression of the archive.

---

### void pause(void)
<p align="right"><code>[SLOT]</code></p>

Pauses the compression of the archive. This slot is **async** and thus you need to wait for the
**paused** signal , Which confirms that the pause call was successfull.

---

### void resume(void)
<p align="right"><code>[SLOT]</code></p>

Resumes the compression of the archive. This slot is **async** and thus you need to wait for the
**resumed** signal, Which confirms that the resume call was successfull.

---

### void cancel(void)
<p align="right"><code>[SLOT]</code></p>

Cancels the compression of the archive. This slot is **async** and thus you need to wait for the
**canceled** signal , Which confirms that the cancel call was successfull.


---

### void progress(QString file , int processedEntries , int totalEntries , qint64 bytesProcessed, qint64 bytesTotal)
<p align="right"><code>[SIGNAL]</code></p>

Emits the progress of the compression.

The signal emits a total of **5 parameters** in which the **first one** is the file that has been compressed ,
the **second** is the number of files **that are compressed** , the **third** is the number of **total files** that is
stagged for compression, the **fourth** is the number of bytes processed and the **last one** is the total number of bytes
required for this compression.


---

### void started(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the compression is started.

---

### void finished(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the compression is finished successfully.

---

### void paused(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the compression is paused successfully.

---

### void resumed(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the compression is resumed successfully.

---

### void canceled(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the compression is canceled successfully.

---

### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when something goes wrong with an archive. Refer the [error codes](QArchiveErrorCodes.md).


