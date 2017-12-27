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


