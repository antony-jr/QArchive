---
id: QArchiveMemoryExtractor
title: Class QArchive::MemoryExtractor
sidebar_label: QArchive::MemoryExtractor
---

The QArchive::MemoryExtractor class helps to extract archives *in memory.*

This class can extract all formats supported by libarchive, this can aslo be used to read the contents of the given 
archive. You can also pass a QIODevice which contains a supported archive in memory. **The QIODevice must not 
be sequential**.



|	    |				               |		
|-----------|------------------------------------------|
|  Header:  | #include < QArchive/QArchive >           |
|   qmake:  | include(QArchive/QArchive.pri)           |
| Class Name| MemoryExtractor
| Namespace | QArchive				       |
| Inherits: | QObject			               |


The class belongs to the QArchive namespace, so make sure to include it.   
**Note:** All functions in this class is **[reentrant](https://doc.qt.io/qt-5/threads-reentrancy.html)**.



### Public Functions

|                                                                                                                  |
|------------------------------------------------------------------------------------------------------------------|
| [MemoryExtractor](#memoryextractorqobject-parent--nullptr--bool-singlethreaded--true)(QObject \*parent = nullptr , bool singleThreaded = true )|
| [MemoryExtractor](#memoryextractorqiodevice-archive--qobject-parent--nullptr--bool-singlethreaded--true)(QIODevice \*, QObject \*parent = nullptr , bool singleThreaded = true )|
| [MemoryExtractor](#memoryextractorconst-qstring-archivepath--qobject-parent--nullptr--bool-singlethreaded--true)(const QString&, QObject \*parent = nullptr,  bool singleThreaded = true )|

### Slots


|           |                                                                                                |
|-----------|------------------------------------------------------------------------------------------------|
| **void**  | [setArchive](#void-setarchiveconst-qstring-archive)(const QString &Archive)                    |
| **void**  | [setArchive](#void-setarchiveqiodevice-archvie)(QIODevice\*)                                     | 
| **void**  | [setCalculateProgress](#void-setcalculateprogressbool-choice)(bool)                            |
| **void**  | [addFilter](#void-addfilterconst-qstring-filter)(const QString&)				     |
| **void**  | [addFilter](#void-addfilterconst-qstringlist-filters)(const QStringList&)                      | 
| **void**  | [setPassword](#void-setpasswordconst-qstring-password)(const QString&)                         |
| **void**  | [setBlocksize](#void-setblocksizeint-size)(int size)                                           |
| **void**  | [getInfo](#void-getinfovoid)(void)                                                             |
| **void**  | [clear](#void-clearvoid)(void)                                                                 |
| **void**  | [start](#void-startvoid)(void)                                                                 |
| **void**  | [pause](#void-pausevoid)(void)                                                                 |
| **void**  | [resume](#void-resumevoid)(void)                                                               |
| **void**  | [cancel](#void-cancelvoid)(void)                                                               |


### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**            | [info](#void-infoqjsonobject-information)(QJsonObject)                          |
| **void**            | [progress](#void-progressqstring-file--int-processedentries--int-totalentries--qint64-bytesprocessed-qint64-bytestotal)(QString, int, int, qint64, qint64)|
| **void**            | [getInfoRequirePassword](#void-getinforequirepasswordint-numberoftries)(int)    |
| **void**            | [extractionRequirePassword](#void-extractionrequirepasswordint-numberoftries)(int)|
| **void**	      | [started](#void-startedvoid)(void)                                              |
| **void**            | [finished](#void-finishedqarchivememoryextractoroutputqarchivedocsqarchivememoryextractoroutputhtml-output)([QArchive::MemoryExtractorOutput](QArchiveMemoryExtractorOutput.md)\*)           |
| **void**            | [paused](#void-pausedvoid)(void)                                                |
| **void**            | [resumed](#void-resumedvoid)(void)                                              |
| **void**            | [canceled](#void-canceledvoid)(void)                                            |
| **void**  	      | [error](#errorshort-errorcodeqarchivedocsqarchiveerrorcodeshtml)(short **[errorCode](QArchiveErrorCodes.md)**)|


## Member Functions Documentation

### MemoryExtractor(QObject \*parent = nullptr , bool singleThreaded = true)

Constructs **MemoryExtractor** and sets the given QObject as the parent.
By default the **MemoryExtractor** is constructed without any parent and runs in a single thread.
The **MemoryExtractor** can be made to run in a different thread if ```singleThreaded``` is set
to false.

```
 QCoreApplication app(argc , argv);
 
 // Default construction , no parent and single threaded
 QArchive::MemoryExtractor Extractor;

 // With parent and single threaded
 auto PExtractor = new QArchive::MemoryExtractor(&app);
 
 // With parent and non-single threaded
 auto PDExtractor = new QArchive::MemoryExtractor(&app , false);
```


---

### MemoryExtractor(QIODevice \*archive , QObject \*parent = nullptr , bool singleThreaded = true)

This is an overloaded constructor. This expects a pointer to a **QIODevice** which will be assumed as 
the archive stagged for extraction.

```
 QCoreApplication app(argc , argv);
 QFile archive("SomeArchive.zip");
 if(!archive.open(QIODevice::ReadOnly)){
	return -1;
 }

 // Default construction , no parent and single threaded
 QArchive::MemoryExtractor Extractor(&archive);
 
 // With parent and single threaded
 auto PExtractor = new QArchive::MemoryExtractor(&archive , &app);
 
 // With parent and non-single threaded
 auto PDExtractor = new QArchive::MemoryExtractor(&archive , &app , false);
```

---

### MemoryExtractor(const QString &archivePath , QObject \*parent = nullptr , bool singleThreaded = true)

This is an overloaded constructor. This expects a QString which is assumed to be the file path of
the archive stagged for extraction.

```
 QCoreApplication app(argc , argv);
 QString archivePath("/tmp/SomeArchive.zip");
 
 // Default construction , no parent and single threaded
 QArchive::MemoryExtractor Extractor(archivePath);
 
 // With parent and single threaded
 auto PExtractor = new QArchive::MemoryExtractor(archivePath ,  &app);
 
 // With parent and non-single threaded
 auto PDExtractor = new QArchive::MemoryExtractor(archivePath , &app , false);

```

---


### void setArchive(const QString &Archive)
<p align="right"><code>[SLOT]</code></p>

Sets the path to archive which is stagged for extraction.

```
 QArchive::MemoryExtractor Extractor;
 Extractor.setArchive("Test.7z");
```

---

### void setArchive(QIODevice \*archive)
<p align="right"><code>[SLOT]</code></p>

Sets the given QIODevice pointer as the archive stagged for extraction. This QIODevice will not 
be deleted by the extractor and the user is responsible of deleting the given pointer.
The given QIODevice has to be opened and readable in order for the extractor to work without
any errors.

```
 QFile *file("Archive.zip");
 file->open(QIODevice::ReadOnly):
 QArchive::MemoryExtractor Extractor;
 
 Extractor.setArchive(file); /* QFile is inherited from QIODevice */
```

---

### void setCalculateProgress(bool choice)
<p align="right"><code>[SLOT]</code></p>

Calculates the progress for the extraction if the given bool is **true**. Please 
see that if this is set , the extraction may take double the time to complete since 
the extractor has to walk through the entire archive in order to know the total number 
of entries in the archive.
But however this can be optimized and normal performance can obtained if you have
called **getInfo** before starting the extraction. But use this strategy only if you
have to see the contents of the archive before extraction.


---

### void addFilter(const QString &filter)
<p align="right"><code>[SLOT]</code></p>

Adds the given file to filter , when the extractor is started , **only the files present in the 
filter will be extracted.**

---

### void addFilter(const QStringList &filters)
<p align="right"><code>[SLOT]</code></p>

Adds the given list of files to the filter , when extractor is started , **only the files present in the
filter will be extracted.**

---

### void setBlocksize(int size)
<p align="right"><code>[SLOT]</code></p>

Sets the blocksize for the archive stagged for extraction.

---

### void setPassword(const QString &Password)
<p align="right"><code>[SLOT]</code></p>

Sets the password for **encrypted archives**. If an archive needs password for extraction then **extractionRequirePassword**
signal will be emitted followed by an error signal. One can avoid password errors and listen to this signals and set password
when needed and start the extraction again. Also **getInfoRequirePassword** is emitted when reading the entries of the 
given archive requires password, however the same procedure has to be followed.

```
 using QArchive::MemoryExtractor;
 MemoryExtractor Extractor("Test.zip");
 
 QObject::connect(&Extractor , &MemoryExtractor::extractionRequirePassword ,
 [&](int NumberOfTries){
    (void)NumberOfTries;
    Extractor.setPassword("Test123");
    Extractor.start();
 });
 
 QObject::connect(&Extractor , &MemoryExtractor::error ,
 [&](short errorCode){
    if(errorCode == QArchive::ArchivePasswordNeeded){
       return; // You can ignore this error.
    }
    
    if(errorCode == QArchive::ArchivePasswordIncorrect){
       return; // Warn and avoid.
    }
    
    /* Do Something. */
 });
 Extractor.start();
```

---

### void getInfo(void)
<p align="right"><code>[SLOT]</code></p>

Gets the information on the entries inside the archive in json format using QJsonObject.
Emits the QJsonObject via the ```info(QJsonObject)``` signal.

---

### void clear(void)
<p align="right"><code>[SLOT]</code></p>

Clears the internal cache.

---

### void start(void)
<p align="right"><code>[SLOT]</code></p>

Starts the extraction of the archive.

---

### void pause(void)
<p align="right"><code>[SLOT]</code></p>

Pauses the extraction of the archive. This slot is **async** and thus you need to wait for the
**paused** signal, Which confirms that the pause call was successfull.

---

### void resume(void)
<p align="right"><code>[SLOT]</code></p>

Resumes the extraction of the archive. This slot is **async** and thus you need to wait for the
**resumed** signal, Which confirms that the resume call was successfull.

---

### void cancel(void)
<p align="right"><code>[SLOT]</code></p>

Cancels the extraction of the archive. This slot is **async** and thus you need to wait for the
**canceled** signal, Which confirms that the cancel call was successfull.

---

### void info(QJsonObject information)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when **[getInfo](#)** method is called. The information is given in QJsonObject format.
The information is typically formated in the down below format with respect to JSON.

```
{
	"<filename>": {
		"BlockSize": <BlockSize>,
		"BlockSizeUnit": <Bytes/KiB> ,
		"Blocks": <Blocks>,
		"FileName": <filename> ,
		"FileType": <RegularFile/Dir/etc>,
		"LastAccessedTime":"2019-01-27T08:21:44",
		"LastModifiedTime":"2019-01-27T08:21:46",
		"LastStatusModifiedTime":"2019-01-27T08:21:46",
		"Size": <Size> ,
		"SizeUnit":<KiB/MiB/GiB>
	},
	.
	.
	.
	// Some more files
}
```

> Note: Everything is a string in the emitted QJsonObject , so you can use QJsonObject::toString()
> method to convert the required information to a QString.
  
--- 

### void progress(QString file , int processedEntries , int totalEntries , qint64 bytesProcessed, qint64 bytesTotal)
<p align="right"><code>[SIGNAL]</code></p>

Emits the progress of the extraction , even if **setCalculateProgress** is set to **false** this signal
will be emitted with only the current extracting file and with no information on the progress , like the
percentage finished and etc.
One has to set **calculate progress** to **true** in order to get meaningful information from the 
emitted integers.

The signal emits a total of **4 parameters** in which the **first one** is the file that has been extracted ,
the **second** is the number of files **that are extracted** , the **third** is the number of **total files** in the
archive, the **fourth** is the number of bytes processed and the **last one** is the total number of bytes
required for this extraction.

---

### void getInfoRequirePassword(int NumberOfTries)
<p align="right"><code>[SIGNAL]</code></p>

When you call **getInfo()** method and if found that the extractor cannot **read** the archive entires 
because it needs a password to do so , This signal is emitted with the total number of tries that
the extractor has already done. A error signal is also emitted after this.

One can avoid password errors and **set the password** and **start** the extraction again when this 
type of signals are emitted or , One can also just exit with password errors.

---

### void extractionRequirePassword(int NumberOfTries)
<p align="right"><code>[SIGNAL]</code></p>

When you call **start()** method and if found that the extractor cannot **extract** the archive entires 
because it needs a **password** to do so , This signal is emitted with the total number of tries that
the extractor has already done. A error signal is also emitted after this.

One can avoid password errors and **set the password** and **start** the extraction again when this 
type of signals are emitted or , One can also just exit with password errors.

---

### void started(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the extraction is started.

---

### void finished([QArchive::MemoryExtractorOutput](QArchiveMemoryExtractorOutput.md) \*output)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the extraction is finished successfully. **The first argument is a [QArchive::MemoryExtractorOutput]() class pointer which can be used to get files which are allocated in memory.** The pointer is **not owned**, you have to delete it after you finished using it. **You only need to delete this ouput pointer and nothing else.**

**Also See:** [QArchive::MemoryExtractorOutput](QArchiveMemoryExtractorOutput.md), [QArchive::MemoryFile](QArchiveMemoryFile.md).

```
QObject::connect(&Extractor, &QArchive::MemoryExtractor::finished,
[&](QArchive::MemoryExtractorOutput *output) {
   auto files = output->getFiles();
   for(auto iter = files.begin(),
	    end = files.end();
	    iter != end;
	    ++iter) {
	auto fileInfo = (*iter).fileInformation();
	auto buffer = (*iter).buffer();
	qDebug() << "FileName:: " << fileInfo.value("FileName").toString();

	buffer->open(QIODevice::ReadOnly);
	qDebug() << "Contents:: " << QString(buffer->readAll());
	buffer->close();	
   }
   /// See QArchive::MemoryExtractorOutput class documentation for
   /// more info.
   output->deleteLater();
   return;
});
```


---

### void paused(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the extraction is paused successfully.

---

### void resumed(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the extraction is resumed successfully.

---

### void canceled(void)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when the extraction is canceled successfully.

---

### error(short **[errorCode](QArchiveErrorCodes.md)**)
<p align="right"><code>[SIGNAL]</code></p>

Emitted when something goes wrong with the given archive. refer the [error codes](QArchiveErrorCodes.md).


