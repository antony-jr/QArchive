---
id: QArchiveMemoryFile
title: Class QArchive::MemoryFile
sidebar_label: QArchive::MemoryFile
---

This class holds information and buffer of a single file extracted by [QArchive::MemoryExtractor](QArchiveMemoryExtractor.md).


|	    |				               |		
|-----------|------------------------------------------|
|  Header:  | #include < QArchive/QArchive >           |
|   qmake:  | include(QArchive/QArchive.pri)           |
| Class Name| MemoryFile                               |
| Namespace | QArchive				       |


The class belongs to the QArchive namespace, so make sure to include it.   


### Public Functions

|                                      |                                                            |
|--------------------------------------|------------------------------------------------------------|
| **QJsonObject const**                | [fileInformation()](#qjsonobject-fileinformation-const)    |
| **QBuffer \* const**                 | [buffer()](#qbuffer-buffer-const)                          |
| **std::shared_ptr\<QBuffer\> const** | [sharedBuffer()](#stdshared_ptrqbuffer-sharedbuffer-const) |


## Member Functions Documentation

### QJsonObject fileInformation() const

Returns file information as QJsonObject. The QJsonObject has the below format w.r.t **JSON.**

```
{
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
}
```

### QBuffer \*buffer() const

<p align="right"><b style="color: red;">(OBSOLETE)</b></b></p>

Returns a QBuffer pointer. This QBuffer is closed by default. You can read or write by opening this buffer. **Do not try to free this pointer. This pointer will be freed automatically when this class is destructed.**

> This is obsolete since v2.2.4 in favor of sharedBuffer()

### std::shared_ptr\<QBuffer\> sharedBuffer() const

Returns a std::shared_ptr QBuffer. This QBuffer is closed by default. You can read or write by opening this buffer.

> Only available in v2.2.4 or later, this is recommended since, you can do whatever with the shared pointer. 
> Even if you free it, he pointer will take care of itself.
