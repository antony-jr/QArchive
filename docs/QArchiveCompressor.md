---
id: QArchiveCompressor
title: Class QArchive::Compressor
sidebar_label: QArchive::Compressor
---

The QArchive::Compressor class helps to create archives that are supported by **libarchive**.
QArchive::Compressor stores a list of files for a single archive at a time , where each slots helps corresponds one action provided by libarchive.

Behind the scenes, QArchive::Compressor uses smart pointers to free libarchive structs automatically and can only compress a single archive , this is to reduce memory usage and to avoid the needless copying of data.

In addition to the above, QArchive::Compressor also provides a async and sync way for the process. QArchive::Compressor is the class you want to use if you want a very simple syntax to create archives in a single line and also forget about thread safety ( *Because it is reentrant by nature* ).

**Note:** All functions in this class is **[reentrant](https://doc.qt.io/qt-5/threads-reentrancy.html)**.


### Public Functions

|                                   |                                                                                   |
|-----------------------------------|-----------------------------------------------------------------------------------|
| **explicit**                      | [Compressor](#explicit-compressorqobject-parent-nullptr)(QObject *parent = nullptr)                                         |
| **explicit**                      | [Compressor](#explicit-compressorconst-qstring-archive)(const QString &Archive)                                            |
| **explicit**                      | [Compressor](#explicit-compressorconst-qstring-archive-const-qstring-file)(const QString &Archive , const QString &file)                      |
| **explicit**                      | [Compressor](#explicit-compressorconst-qstring-archive-const-qstringlist-files)(const QString &Archive , const QStringList &files)                 |
| **Compressor&**                   | [setArchive](#compressor-setarchiveconst-qstring-archive)(const QString &Archive)                                            |
| **Compressor&**                   | [setArchive](#compressor-setarchiveconst-qstring-archive-const-qstring-file)(const QString &Archive , const QString &file)                      |
| **Compressor&**                   | [setArchive](#compressor-setarchiveconst-qstring-archive-const-qstringlist-files)(const QString &Archive , const QStringList &files)                 |
| **Compressor&**                   | [setArchiveFormat](#compressor-setarchiveformatshort-archiveformat-qarchive-docs-qarchiveformatshtml)(short **[ArchiveFormat](QArchiveFormats.md)** )              |
| **Compressor&**                   | [setPassword](#compressor-setpasswordconst-qstring-password)(const QString &Password)                                          |
| **Compressor&**                   | [setBlocksize](#compressor-setblocksizeint-size)(int size)                                                        |
| **Compressor&**                   | [addFiles](#compressor-addfilesconst-qstring-file)(const QString &file)                                                 |
| **Compressor&**                   | [addFiles](#compressor-addfilesconst-qstringlist-files)(const QStringList &files)                                            |
| **Compressor&**                   | [removeFiles](#compressor-removefilesconst-qstring-file)(const QString &file)                                              |
| **Compressor&**                   | [removeFiles](#compressor-removefilesconst-qstringlist-files)(const QStringList &files)                                         |
| **Compressor&**                   | [clear](#compressor-clearvoid)(void)                                                                   | 


### Slots

|                   |                                                                                                |
|-------------------|------------------------------------------------------------------------------------------------|
| **Compressor&**  	| [waitForFinished](#compressor-waitforfinishedvoid)(void)                                        |
| **Compressor&**    | [start](#compressor-startvoid)(void)                                                            |
| **Compressor&**    | [pause](#compressor-pausevoid)(void)                                                            |
| **Compressor&**    | [resume](#compressor-resumevoid)(void)                                                          |
| **Compressor&**    | [cancel](#compressor-cancelvoid)(void)                                                          |
| **const bool**    | [isRunning](#bool-isrunningvoid-const)(void)                                                   |
| **const bool**    | [isCanceled](#bool-iscanceledvoid-const)(void)                                                 |
| **const bool**    | [isPaused](#bool-ispausedvoid-const)(void)                                                     |
| **const bool**    | [isStarted](#bool-isstartedvoid-const)(void)                                                   |
| **Compressor&**    | [setFunc](#compressor-setfuncshort-signalcode-qarchive-docs-qarchivesignalcodeshtml-std-function-voidvoid-function)(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(void)> function)                                                |
| **Compressor&**    | [setFunc](#compressor-setfuncshort-signalcode-qarchive-docs-qarchivesignalcodeshtml-std-function-voidqstring-function)(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(QString)> function)|
| **Compressor&**   | [setFunc](#compressor-setfuncstd-function-voidint-function)(std::function<void(int)> function)                                                 |
| **Compressor&**   | [setFunc](#compressor-setfuncstd-function-voidshort-qstring-function)(std::function<void(short,QString)> function) |


### Signals

|                     |                                                                                 |
|---------------------|---------------------------------------------------------------------------------|
| **void**	          | [started](#void-startedvoid)(void)								                |
| **void**            | [finished](#void-finishedvoid)(void)                        	                |
| **void**            | [paused](#void-pausedvoid)(void)                                                |
| **void**            | [resumed](#void-resumedvoid)(void)                                              |
| **void**            | [canceled](#void-canceledvoid)(void)                                            |
| **void**            | [progress](#void-progressint-percentage)(int progress)                                                      |
| **void**    	      | [compressing](#void-compressingconst-qstring-archive)(const QString& archive)   |
| **void**    	      | [compressed](#void-compressedconst-qstring-archive)(const QString& archive)       |
| **void**  	      | [error](#errorshort-errorcode-qarchive-docs-qarchiveerrorcodeshtml-const-qstring-file)(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)|


## Member Functions Documentation

### explicit Compressor(QObject *parent = nullptr)

Constructs **Compressor** and sets the **QObject parent**. 
If the user did not give any parent , The Compressor will be constructed 
without any parent.

```
 QObject *parent = new QObject;
 QArchive::Compressor Compressor;
 QArchive::Compressor Compressor(parent); // With parent;
```


### explicit Compressor(const QString &Archive)

Constructs **Compressor** and Sets the **Archive Path** as the given **QString**.
This asigns no parent for the **Compressor**. This also finds the archive [archive
format](QArchiveFormats.md) automatically if extension is given.

```
 QArchive::Compressor Compressor("Test.7z");
```

### explicit Compressor(const QString &Archive , const QString &file) 

Constructs **Compressor** and Sets the **Archive Path** and add **a single file**  
as the given **QString**'s. This asigns no parent for the **Compressor**.

```
 QArchive::Compressor Compressor("Test.7z" , "MyFiles/Output.txt");
```

> Please See that if you add a file directly like 'MyFiles/Ouput.txt' then it will be added
without a parent directory and so in the archive it will be added in the root
Directories can only be added as a whole and not partially.

### explicit Compressor(const QString &Archive , const QStringList &files) 

Constructs **Compressor** and Sets the **Archive Path** and adds **a list of file(s)**  
as the given **QString**'s. This asigns no parent for the **Compressor**.

```
 QArchive::Compressor Compressor("Test.7z" ,QStringList()<<"MyFiles/Output.txt"<<"MyDir");
```

> Please See that if you add a file directly like 'MyFiles/Ouput.txt' then it will be added
without a parent directory and so in the archive it will be added in the root
Directories can only be added as a whole and not partially.

### Compressor& setArchive(const QString &Archive)

Sets the **Archive Path** as the given **QString**.

```
 Compressor.setArchive("Test.7z");
```

### Compressor& setArchive(const QString &Archive , const QString &file)

Sets the **Archive Path** and adds **a single file** as the given **QString**'s.

```
 Compressor.setArchive("Test.7z" , "MyFiles/Output.txt");
```

> Please See that if you add a file directly like 'MyFiles/Ouput.txt' then it will be added
without a parent directory and so in the archive it will be added in the root
Directories can only be added as a whole and not partially.

### Compressor& setArchive(const QString &Archive , const QStringList &files)

Sets the **Archive Path** and adds **a list of file(s)** as the given **QString**'s.

```
 Compressor.setArchive("Test.7z" ,QStringList()<<"MyFiles/Output.txt"<<"MyDir");
```

> Please See that if you add a file directly like 'MyFiles/Ouput.txt' then it will be added
without a parent directory and so in the archive it will be added in the root
Directories can only be added as a whole and not partially.

### Compressor& setArchiveFormat(short **[ArchiveFormat](QArchiveFormats.md)**)

Sets the archive format with respect to the [archive format codes](QArchiveFormats.md).


### Compressor& setBlocksize(int size)

Sets the **Archive Blocksize**.


### Compressor& setPassword(const QString &Password)

Sets the password for **archives**. Which will result in a encrypted archive.

> Note: libarchive does not support this fully so most of the time this will
not work.


### Compressor& addFiles(const QString &file)

Adds **a single file** as the given **QString**.

> Please See that if you add a file directly like 'MyFiles/Ouput.txt' then it will be added
without a parent directory and so in the archive it will be added in the root
Directories can only be added as a whole and not partially.

### Compressor& addFiles(const QStringList &files)

Adds **a list of file(s)** as the given **QStringList**.

> Please See that if you add a file directly like 'MyFiles/Ouput.txt' then it will be added
without a parent directory and so in the archive it will be added in the root
Directories can only be added as a whole and not partially.

### Compressor& removeFiles(const QString &file)

Removes a **single file** from the archive.

### Compressor& removeFiles(const QStringList &files)

Removes a **a list of file(s)** from the archive.


### Compressor& clear(void)

Clears the internal cache.

```
 Compressor.clear();
```

### Compressor& waitForFinished(void)
This member function is a **[SLOT]**

Blocks the caller thread until the process if **finished** or **canceled**.

```
 QArchive::Compressor("Test.7z" , "MyFiles/Output.txt").start().waitForFinished();
```

### Compressor& start(void)
This member function is a **[SLOT]**

Starts the compression of the file(s).

### Compressor& pause(void)
This member function is a **[SLOT]**

Pauses the compression of the file(s). This slot is **asyc** and thus you need to wait for the
**paused** signal , Which confirms that the pause call was successfull.

### Compressor& resume(void)
This member function is a **[SLOT]**

Resumes the compression of the file(s). This slot is **asyc** and thus you need to wait for the
**resumed** signal , Which confirms that the resume call was successfull.

### Compressor& cancel(void)
This member function is a **[SLOT]**

Cancels the compression of the file(s). This slot is **asyc** and thus you need to wait for the
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

### Compressor& setFunc(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(void)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the signal with respect to the **[signalCode](QArchiveSignalCodes.md)**.
**started,finished,resumed,paused and canceled signals are only signals which can be connect with this method**.

```
 Compressor.setFunc(QArchive::FINISHED , [&](){
    qDebug() << "Finished compression!";
    // quit.
 });
```
 

### Compressor& setFunc(short **[signalCode](QArchiveSignalCodes.md)** , std::function<void(QString)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the **compressing** and **compressed** signal with respect to the **[signalCode](QArchiveSignalCodes.md)**.

```
 Compressor.setFunc(QArchive::COMPRESSING , [&](QString file){
    qDebug() << "Compressing:: " << file;
 });
```
 
### Compressor& setFunc(std::function<void(int)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the **progress** signal.

```
 Compressor.setFunc([&](int prog){
    qDebug() << "Progress:: " << prog << " % Done.";
 });
```
 
### Compressor& setFunc(std::function<void(short,QString)> function)
This member function is a **[SLOT]**

Connects the **lambda** function to the **error signal**. Refer the **[error codes](QArchiveErrorCodes.md)** for more information on
codes passed by the **error signal**.

```
 Compressor.setFunc([&](short errorCode , QString eMsg){
    qDebug() << eMsg << " :: " << errorCode;
 });
```
 
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

### void progress(int percentage)
This member function is a **[SIGNAL]**

Emitted when there is a progress update.


### void compressing(const QString &Archive)
This member function is a **[SIGNAL]**

Emitted when a file from archive is started compression.

### void compressed(const QString &Archive)
This member function is a **[SIGNAL]**

Emitted when a file from the archive is compressed successfully!

### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with an archive. refer the [error codes](QArchiveErrorCodes.md).
