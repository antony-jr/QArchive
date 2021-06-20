---
id: QArchiveMemoryExtractorOutput
title: Class QArchive::MemoryExtractorOutput
sidebar_label: QArchive::MemoryExtractorOutput
---


This class is emitted as the output of [**QArchive::MemoryExtractor**](QArchiveMemoryExtractor.md) class in the finished signal. This class has only one method which gives a vector of [**QArchive::MemoryFile**](QArchiveMemoryFile.md)


|	    |				               |		
|-----------|------------------------------------------|
|  Header:  | #include < QArchive/QArchive >           |
|   qmake:  | include(QArchive/QArchive.pri)           |
| Class Name| MemoryExtractorOutput 		       |
| Namespace | QArchive				       |
| Inherits: | QObject			               |


The class belongs to the QArchive namespace, so make sure to include it.   
**Note:** All functions in this class is **[reentrant](https://doc.qt.io/qt-5/threads-reentrancy.html)**.



### Public Functions

|                                                                                       |
|---------------------------------------------------------------------------------------|
| [MemoryExtractorOutput]()(QObject \*parent = nullptr)					|


### Slots


|                                                                   |                                            |
|-------------------------------------------------------------------|--------------------------------------------|
| **QVector<[QArchive::MemoryFile](QArchiveMemoryFile.md)>& const** | [getFiles()](#qvectorqarchivememoryfileqarchivedocsqarchivememoryfilehtml-getfiles-const)|


## Member Functions Documentation

### MemoryExtractorOutput(QObject \*parent = nullptr)

Default constructor of MemoryExtractorOutput. It's highly recommended that you never construct this class yourself.

---

### QVector<[QArchive::MemoryFile](QArchiveMemoryFile.md)>& getFiles() const

Returns a **QVector** of [QArchive::MemoryFile](QArchiveMemoryFile.md).

