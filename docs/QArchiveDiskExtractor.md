---
id: QArchiveDiskExtractor
title: Class QArchive::DiskExtractor
sidebar_label: QArchive::DiskExtractor
---

The QArchive::DiskExtractor class helps to extract archives that are based on the disk, (i.e) The storage of the computer.
This class can extract all formats supported by libarchive.

**Note:** All functions in this class is **[reentrant](https://doc.qt.io/qt-5/threads-reentrancy.html)**.



### Public Functions

|                                                                                   |
|-----------------------------------------------------------------------------------|
| [DiskExtractor](#)(bool singleThreaded = true , )  |
| [DiskExtractor](#)(const QString &ArchivePath , )                                        |
| **explicit**                      | [Extractor](#explicit-extractorconst-qstring-archive-const-qstring-destination)(const QString &Archive , const QString &Destination)|

### Slots


|                   |                                                                                                |
|-------------------|------------------------------------------------------------------------------------------------|
| **Extractor&**                    | [setArchive](#extractor-setarchiveconst-qstring-archive)(const QString &Archive)  |
| **Extractor&**                    | [setArchive](#extractor-setarchiveconst-qstring-archive-const-qstring-destination)(const QString &Archive , const QString &Destination)|
| **Extractor&**                    | [setPassword](#extractor-setpasswordconst-qstring-password)(const QString &Password)|
| **Extractor&**                    | [setAskPassword](#extractor-setaskpasswordbool-choose)(bool choose)               |
| **Extractor&**                    | [setBlocksize](#extractor-setblocksizeint-size)(int size)                         |
| **Extractor&**                    | [onlyExtract](#extractor-onlyextractconst-qstring-memberinarchive)(const QString &MemberInArchive)|
| **Extractor&**                    | [onlyExtract](#extractor-onlyextractconst-qstringlist-membersinarchive)(const QStringList &MembersInArchive)|
| **Extractor&**                    | [clear](#extractor-clearvoid)(void)                                               |
| **Extractor&**    | [start](#extractor-startvoid)(void)                                                            |
| **Extractor&**    | [pause](#extractor-pausevoid)(void)                                                            |
| **Extractor&**    | [resume](#extractor-resumevoid)(void)                                                          |
| **Extractor&**    | [cancel](#extractor-cancelvoid)(void)                                                          |


### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**	          | [started](#void-startedvoid)(void)								                |
| **void**            | [finished](#void-finishedvoid)(void)                        	                |
| **void**            | [paused](#void-pausedvoid)(void)                                                |
| **void**            | [resumed](#void-resumedvoid)(void)                                              |
| **void**            | [canceled](#void-canceledvoid)(void)                                            |
| **void**            | [progress](#void-progressint-percentage)(int percentage)                        |
| **void**            | [passwordRequired](#void-passwordrequiredint-tries)(int tries)                  |
| **void**    	      | [extracting](#void-extractingconst-qstring-archive)(const QString& archive)     |
| **void**    	      | [extracted](#void-extractedconst-qstring-archive)(const QString& archive)       |
| **void**  	      | [error](#errorshort-errorcode-qarchive-docs-qarchiveerrorcodeshtml-const-qstring-file)(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)|


## Member Functions Documentation

### explicit Extractor(QObject *parent = nullptr)

Constructs **Extractor** and sets the **QObject parent**. 
If the user did not give any parent , The Extractor will be constructed 
without any parent.

```
 QObject *parent = new QObject;
 QArchive::Extractor Extractor;
 QArchive::Extractor Extractor(parent); // With parent;
```


### explicit Extractor(const QString &Archive)

Constructs **Extractor** and Sets the **Archive Path** as the given **QString**.
This asigns no parent for the **Extractor**.

```
 QArchive::Extractor Extractor("Test.7z");
```

### explicit Extractor(const QString &Archive , const QString &Destination) 

Constructs **Extractor** and Sets the **Archive Path** and the **Destination Path**  
as the given **QString**'s. This asigns no parent for the **Extractor**.

```
 QArchive::Extractor Extractor("Test.7z" , "MyArchives/Extracted");
```

### Extractor& setArchive(const QString &Archive)

Sets the **Archive Path** as the given **QString**.

```
 Extractor.setArchive("Test.7z");
```

### Extractor& setArchive(const QString &Archive , const QString &Destination)

Sets the **Archive Path** and the **Destination Path** as the given **QString**'s.

```
 Extractor.setArchive("Test.7z" , "MyArchives/Extracted");
```

### Extractor& setBlocksize(int size)

Sets the **Archive Blocksize**.


### Extractor& setPassword(const QString &Password)

Sets the password for **encrypted archives**.


### Extractor& setAskPassword(bool choose)

If **choose** is **True** then on wrong password or when password is needed for 
an archive it emits **passwordRequired(int)** signal , with the number of tries
as the argument. The password can be set with **setPassword(QString)** slot.
If an **empty** password is set with **setPassword(QString)** slot then it will 
result in a **QArchive::ARCHIVE_WRONG_PASSWORD** or **QArchive::ARCHIVE_PASSWORD_NOT_GIVEN**
if the archive is **encrypted**.

**Note**: 'passwordRequired' signal will be emitted until the password is empty or correct.
'passwordRequired' signal **will not be emitted** if **setAskPassword** is set to **False**.


### Extractor& onlyExtract(const QString &MemberInArchive)

Only extracts the specified **member** in the archive.

```
 Extractor.onlyExtract("Ouput.txt");
```

### Extractor& onlyExtract(const QStringList &MembersInArchive)

Only extracts the list of files in the given **QStringList**.

```
 Extractor.onlyExtract(QStringList() << "Ouput.txt" << "MyTools");
```

### Extractor& clear(void)

Clears the internal cache.

```
 Extractor.clear();
```

### Extractor& waitForFinished(void)
This member function is a **[SLOT]**

Blocks the caller thread until the process if **finished** or **canceled**.

```
 QArchive::Extractor("Test.7z" , "MyArchives/Extracted").start().waitForFinished();
```

### Extractor& start(void)
This member function is a **[SLOT]**

Starts the extraction of the archive.

### Extractor& pause(void)
This member function is a **[SLOT]**

Pauses the extraction of the archive. This slot is **asyc** and thus you need to wait for the
**paused** signal , Which confirms that the pause call was successfull.

### Extractor& resume(void)
This member function is a **[SLOT]**

Resumes the extraction of the archive. This slot is **asyc** and thus you need to wait for the
**resumed** signal , Which confirms that the resume call was successfull.

### Extractor& cancel(void)
This member function is a **[SLOT]**

Cancels the extraction of the archive. This slot is **asyc** and thus you need to wait for the
**canceled** signal , Which confirms that the cancel call was successfull.

### bool isRunning(void) const
This member function is a **[SLOT]**

Returns **True** if the process is running.

### bool isCanceled(void) const
This member function is a **[SLOT]**

Returns **True** if the process has been canceled.

### bool isPaused(void) const
This member function is a **[SLOT]**

Returns **True** if the process is paused.

### bool isStarted(void) const
This member function is a **[SLOT]**

Returns **True** if the process has been started.

### Extractor& setFunc(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(void)> function)
This member function is a [SLOT]

Connects the **lambda** function to the signal with respect to the **[signalCode](QArchiveSignalCodes.md)**.
**started,finished,resumed,paused and canceled signals are only signals which can be connect with this method**.

```
 Extractor.setFunc(QArchive::FINISHED , [&](){
    qDebug() << "Finished extraction!";
 });
```

### Extractor& setFunc(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(QString)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the extracting or extracted signal with respect to the **[signalCode](QArchiveSignalCodes.md)**.

```
 Extractor.setFunc(QArchive::EXTRACTING , [&](QString file){
    qDebug() << "Extracting:: " << file;
 });
```
 
### Extractor& setFunc(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(int)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the **progress** or **passwordRequired** signal with respect
to the **[signalCode](QArchiveSignalCodes.md)**.


```
 Extractor.setFunc(QArchive::PROGRESS , [&](int prog){
    qDebug() << "Progress:: " << prog << " % Done.";
 });
```
 
### Extractor& setFunc(std::function<void(short,QString)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the **error signal**. Refer the **[error codes](QArchiveErrorCodes.md)** for more information on
codes passed by the **error signal**.

```
 Extractor.setFunc([&](short errorCode , QString eMsg){
    qDebug() << eMsg << " :: " << errorCode;
 });
```
 
### void started(void)
This member function is a **[SIGNAL]**

Emitted when the extraction is started.

### void finished(void)
This member function is a **[SIGNAL]** 

Emitted when the extraction is finished successfully.

### void paused(void)
This member function is a **[SIGNAL]** 

Emitted when the extraction is paused successfully.

### void resumed(void)
This member function is a **[SIGNAL]** 

Emitted when the extraction is resumed successfully.

### void canceled(void)
This member function is a **[SIGNAL]** 

Emitted when the extraction is canceled successfully.

### void progress(int percentage)
This member function is a **[SIGNAL]**

Emitted when there is a progress update.

### void passwordRequired(int tries)
This member function is a **[SIGNAL]**

Emitted when password is required for an archive with the number of
**tries** as the argument.

**Note**: 'passwordRequired' signal will be emitted until the password is empty or correct.
'passwordRequired' signal **will not be emitted** if **setAskPassword** is set to **False**.
The password should be **set** using **setPassword** slot. 


### void extracting(const QString &Archive)
This member function is a **[SIGNAL]**

Emitted when a file from archive is started extraction.

### void extracted(const QString &Archive)
This member function is a **[SIGNAL]**

Emitted when a file from the archive is extracted successfully!

### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with an archive. refer the [error codes](QArchiveErrorCodes.md).


