---
id: QArchiveReader
title: Class QArchive::Reader
sidebar_label: QArchive::Reader
---

The QArchive::Reader class helps to read archives that are supported by **libarchive**.
QArchive::Reader stores a single archive at a time , where each slots helps corresponds one action provided by libarchive.

Behind the scenes, QArchive::Reader uses smart pointers to free libarchive structs automatically and can only read a single archive , this is to reduce memory usage and to avoid the needless copying of data.   

In addition to the above, QArchive::Reader also provides a async and sync way for the process. QArchive::Reader is the class you want to use if you want a very simple syntax to read archives in a single line and also forget about thread safety ( *Because it is reentrant by nature* ).

**Note:** All functions in this class is **[reentrant](https://doc.qt.io/qt-5/threads-reentrancy.html)**.


### Public Functions

|                                   |                                                                                   |
|-----------------------------------|-----------------------------------------------------------------------------------|
| **explicit**                      | [Reader](#explicit-readerqobject-parent-nullptr)(QObject *parent = nullptr) |
| **explicit**                      | [Reader](#explicit-readerconst-qstring-archive)(const QString &Archive)     |
| **Reader&**                       | [setArchive](#reader-setarchiveconst-qstring-archive)(const QString &Archive)     |
| **Reader&**                       | [setPassword](#reader-setpasswordconst-qstring-password)(const QString &Password) |
| **Reader&**                       | [setAskPassword](#reader-setaskpasswordbool-choose)(bool choose)                  |
| **Reader&**                       | [setBlocksize](#reader-setblocksizeint-size)(int size)                            |
| **QJsonObject**                   | [getFilesList](#reader-getfileslistvoid)(void)                                                            |
| **Reader&**                       | [clear](#reader-clearvoid)(void)                                                  |


### Slots

|                   |                                                                                                |
|-------------------|------------------------------------------------------------------------------------------------|
| **Reader&**  	| [waitForFinished](#reader-waitforfinishedvoid)(void)                                        |
| **Reader&**    | [start](#reader-startvoid)(void)                                                            |
| **Reader&**    | [pause](#reader-pausevoid)(void)                                                            |
| **Reader&**    | [resume](#reader-resumevoid)(void)                                                          |
| **Reader&**    | [cancel](#reader-cancelvoid)(void)                                                          |
| **const bool**    | [isRunning](#bool-isrunningvoid-const)(void)                                                   |
| **const bool**    | [isCanceled](#bool-iscanceledvoid-const)(void)                                                 |
| **const bool**    | [isPaused](#bool-ispausedvoid-const)(void)                                                     |
| **const bool**    | [isStarted](#bool-isstartedvoid-const)(void)                                                   |
| **Reader&**    | [setFunc](#reader-setfuncshort-signalcode-qarchive-docs-qarchivesignalcodeshtml-std-function-voidvoid-function)(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(void)> function)                                                |
| **Reader&**    | [setFunc](#reader-setfuncstd-function-voidqjsonobject-function)(std::function<void(QJsonObject)> function)                                        |
| **Reader&**    | [setFunc](#reader-setfuncstd-function-voidint-function)(std::function<void(int)> function)|
| **Reader&**    | [setFunc](#reader-setfuncstd-function-voidshort-qstring-function)(std::function<void(short,QString)> function)                                           |


### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**	          | [started](#void-startedvoid)(void)								                |
| **void**            | [finished](#void-finishedvoid)(void)                        	                |
| **void**            | [paused](#void-pausedvoid)(void)                                                |
| **void**            | [resumed](#void-resumedvoid)(void)                                              |
| **void**            | [canceled](#void-canceledvoid)(void)                                            |
| **void**            | [passwordRequired](#void-passwordrequiredint-tries)(int tries)                  |
| **void**            | [filesList](#void-fileslistqjsonobject-files)(QJsonObject files)                                                |
| **void**  	      | [error](#errorshort-errorcode-qarchive-docs-qarchiveerrorcodeshtml-const-qstring-file)(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)|


## Member Functions Documentation

### explicit Reader(QObject *parent = nullptr)

Constructs **Reader** and sets the **QObject parent**. 
If the user did not give any parent , The Reader will be constructed 
without any parent.

```
 QObject *parent = new QObject;
 QArchive::Reader Reader;
 QArchive::Reader Reader(parent); // With parent;
```


### explicit Reader(const QString &Archive)

Constructs **Reader** and Sets the **Archive Path** as the given **QString**.
This asigns no parent for the **Reader**.

```
 QArchive::Reader Reader("Test.7z");
```


### Reader& setArchive(const QString &Archive)

Sets the **Archive Path** as the given **QString**.

```
 Reader.setArchive("Test.7z");
```


### Reader& setBlocksize(int size)

Sets the **Archive Blocksize**.


### Reader& setPassword(const QString &Password)

Sets the password for **encrypted archives**.


### Reader& setAskPassword(bool choose)

If **choose** is **True** then on wrong password or when password is needed for 
an archive it emits **passwordRequired(int)** signal , with the number of tries
as the argument. The password can be set with **setPassword(QString)** slot.
If an **empty** password is set with **setPassword(QString)** slot then it will 
result in a **QArchive::ARCHIVE_WRONG_PASSWORD** or **QArchive::ARCHIVE_PASSWORD_NOT_GIVEN**
if the archive is **encrypted**.

**Note**: 'passwordRequired' signal will be emitted until the password is empty or correct.
'passwordRequired' signal **will not be emitted** if **setAskPassword** is set to **False**.


### Reader& getFilesList(void)

Returns the list of files in a **QJsonObject**.It returns empty if something bad happend.
The json object follows the following structure to carry files information with it.

```
  QJsonObject(
    {
        "Output.txt":
        {
        "BlockSize":0,
        "BlockSizeUnit":"Bytes",
        "Blocks"0,
        "FileName":"Output.txt",
        "FileType":"RegularFile",
        "LastAccessedTime":"Unknown",
        "LastModifiedTime":"2018-04-13T13:37:13",
        "LastStatusModifiedTime":"Unknown",
        "Size":13,
        "SizeUnit":"Bytes"
        }
    }
  )
```

**Where**,

```
Output.txt - Is the path inside the archive.
BlockSize  - Is the blocksize of the file inside the archive.
BlockSizeUnit - Is the unit which the blocksize is represented.
Blocks - Is the number of I/O blocks of the file inside the archive.
FileName - Is the filename of the entry in the archive.
FileType - Filetype represents the type of file , These are following
                 types passed by the reader.
RegularFile , A normal file.
Directory   , A Directory.
CharacterDevice  , A Character Device.
BlockDevice , A Block Device.
NamedPipe , A Named Pipe.

LastAccessedTime - Represents the last accessed time in Qt::ISODate.
LastModifiedTime - Represents the last modified time in Qt::ISODate.
LastStatusModifiedTime - Represents the last status modified time in Qt::ISODate.
Size - Is the size of the file.
SizeUnit - Is the units which **size** is represented.
```
                 
### Reader& clear(void)

Clears the internal cache.

```
 Reader.clear();
```

### Reader& waitForFinished(void)
This member function is a **[SLOT]**

Blocks the caller thread until the process if **finished** or **canceled**.

```
 qDebug() << QArchive::Reader("Test.7z").start().waitForFinished().getFilesList();
```

### Reader& start(void)
This member function is a **[SLOT]**

Starts the reading of the archive.

### Reader& pause(void)
This member function is a **[SLOT]**

Pauses the reading of the archive. This slot is **asyc** and thus you need to wait for the
**paused** signal , Which confirms that the pause call was successfull.

### Reader& resume(void)
This member function is a **[SLOT]**

Resumes the reading of the archive. This slot is **asyc** and thus you need to wait for the
**resumed** signal , Which confirms that the resume call was successfull.

### Reader& cancel(void)
This member function is a **[SLOT]**

Cancels the reading of the archive. This slot is **asyc** and thus you need to wait for the
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

### Reader& setFunc(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(void)> function)
This member function is a [SLOT]

Connects the **lambda** function to the signal with respect to the **[signalCode](QArchiveSignalCodes.md)**.
**started,finished,resumed,paused and canceled signals are only signals which can be connect with this method**.

```
 Reader.setFunc(QArchive::FINISHED , [&](){
    qDebug() << "Finished reading!";
 });
```


### Reader& setFunc(std::function<void(QJsonObject)> function)
This member function is a [SLOT]

Connects the **lambda** function to the **filesList** signal.


### Reader& setFunc(std::function<void(int)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the **passwordRequired** signal.


```
 Reader.setFunc([&](int Tries){
    Reader.setPassword("Something");
 });
```
 
### Reader& setFunc(std::function<void(short,QString)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the **error signal**. Refer the **[error codes](QArchiveErrorCodes.md)** for more information on
codes passed by the **error signal**.

```
 Reader.setFunc([&](short errorCode , QString eMsg){
    qDebug() << eMsg << " :: " << errorCode;
 });
```
 
### void started(void)
This member function is a **[SIGNAL]**

Emitted when the reading is started.

### void finished(void)
This member function is a **[SIGNAL]** 

Emitted when the reading is finished successfully.

### void paused(void)
This member function is a **[SIGNAL]** 

Emitted when the reading is paused successfully.

### void resumed(void)
This member function is a **[SIGNAL]** 

Emitted when the reading is resumed successfully.

### void canceled(void)
This member function is a **[SIGNAL]** 

Emitted when the reading is canceled successfully.

### void passwordRequired(int tries)
This member function is a **[SIGNAL]**

Emitted when password is required for an archive with the number of
**tries** as the argument.

**Note**: 'passwordRequired' signal will be emitted until the password is empty or correct.
'passwordRequired' signal **will not be emitted** if **setAskPassword** is set to **False**.
The password should be **set** using **setPassword** slot. 

### void filesList(QJsonObject files)
This member function is a **[SIGNAL]**

Emitted with the list of files in a **QJsonObject** when finished successfully!
The **QJsonObject** follows the below structure to carry information about the files.


```
  QJsonObject(
    {
        "Output.txt":
        {
        "BlockSize":0,
        "BlockSizeUnit":"Bytes",
        "Blocks"0,
        "FileName":"Output.txt",
        "FileType":"RegularFile",
        "LastAccessedTime":"Unknown",
        "LastModifiedTime":"2018-04-13T13:37:13",
        "LastStatusModifiedTime":"Unknown",
        "Size":13,
        "SizeUnit":"Bytes"
        }
    }
  )
```

**Where**,

```
Output.txt - Is the path inside the archive.
BlockSize  - Is the blocksize of the file inside the archive.
BlockSizeUnit - Is the unit which the blocksize is represented.
Blocks - Is the number of I/O blocks of the file inside the archive.
FileName - Is the filename of the entry in the archive.
FileType - Filetype represents the type of file , These are following
                 types passed by the reader.
RegularFile , A normal file.
Directory   , A Directory.
CharacterDevice  , A Character Device.
BlockDevice , A Block Device.
NamedPipe , A Named Pipe.

LastAccessedTime - Represents the last accessed time in Qt::ISODate.
LastModifiedTime - Represents the last modified time in Qt::ISODate.
LastStatusModifiedTime - Represents the last status modified time in Qt::ISODate.
Size - Is the size of the file.
SizeUnit - Is the units which **size** is represented.
```
                 
### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with an archive. refer the [error codes](QArchiveErrorCodes.md).
