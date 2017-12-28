---
id: QArchiveExtractor
title: Class QArchive::Extractor
sidebar_label: QArchive::Extractor
---

The Extractor Takes care of **extraction** of archives with the help of libarchive.   
This class runs on a seperate thread to avoid blocking by **libarchive**.

### Public Functions

|                                   |                                                                                   |
|-----------------------------------|-----------------------------------------------------------------------------------|
| **explicit**                      | Extractor(QObject *parent = NULL)                                                 |
| **explicit**                      | Extractor(const QString& archive)                                                 |
| **explicit**                      | Extractor(const QStringList& archives)                                            |
| **explicit**                      | Extractor(const QString& archive , const QString& destination)                    |
| **explicit**                      | Extractor(const QStringList& archives , const QString& destination)               |
| **void**                          | addArchive(const QString& archive)                                                |
| **void**                          | addArchive(const QStringList& archives)                                           |
| **void**                          | removeArchive(const QString& archive)                                             |
| **void**                          | setDestination(const QString& destination)                                        |

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
| **void**            | finished(void)                        	                                        |
| **void**    	      | extracting(const QString& archive)                                              |
| **void**    	      | extracted(const QString& archive)      	                                        |
| **void**            | status(const QString& archive ,const QString& file)                             |
| **void**  	      | error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)	    |    


## Member Functions Documentation

#### explicit Extractor(QObject *parent = NULL)

Constructs **Extractor** and sets the **QObject parent**.

#### explicit Extractor(const QString& archive)

Constructs **Extractor** and points to the archive given by the user , this sets the QObject parent as **0**

#### explicit Extractor(const QStringList& archives)

This is an overloaded function. This points to a set of archive given by the user.

#### explicit Extractor(const QString& archive , const QString& destination) 

Constructs **Extractor** , points to the archive given by the user and then set the destination where   
the archive will be extracted.

#### explicit Extractor(const QStringList& archives , const QString& destination) 

This is an overloaded function. This points to a set of archive given by the user.

#### void addArchive(const QString& archive)

This adds a archive to the queue.

#### void addArchive(const QStringList& archives)

This is an overloaded function. Adds a list of archives to the queue.

#### void removeArchive(const QString& archive)

Removes an archive from the queue.

#### void setDestination(const QString& destination)

Sets the destination where the archive(s) will be extracted. Default is **the programs working directory.**

#### void start(void)
This member function is a **[SLOT]**

Starts the extraction of all archives in the queue. Inherited from **[QThread](http://doc.qt.io/qt-5/qthread.html)**.

#### void quit(void)
This member function is a **[SLOT]**

Quits the extraction thread. Inherited from **[QThread](http://doc.qt.io/qt-5/qthread.html)**

#### void finished(void)
This member function is a **[SIGNAL]** 

Emitted when all jobs are done. (i.e) When all extraction is finished.

#### void extracting(const QString& archive)
This member function is a **[SIGNAL]**

Emitted when an archive is started extraction from the queue.

#### void extracted(const QString& archive)
This member function is a **[SIGNAL]**

Emitted when an archive is extracted successfully!

#### void status(const QString& archive , const QString& file)
This member function is a **[SIGNAL]**

Emitted when a single file is extracted from an archive that is extracting from the queue.

#### error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)
This member function is a **[SIGNAL]**

Emitted when something goes wrong with an archive. refer the [error codes](QArchiveErrorCodes.md)


