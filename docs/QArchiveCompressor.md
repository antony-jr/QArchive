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
| **void**    	      | compressing(const QString& file)                                                |
| **void**    	      | compressed(const QString& file)                                       	        |
| **void**  	      | error(short **[errorCode](QArchiveErrorCodes.md)** , const QString& file)	    |   
