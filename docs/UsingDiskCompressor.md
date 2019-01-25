---
id: UsingDiskCompressor
title: Using QArchive to Compress a set of files and directories.
sidebar_label: Compressing files with QArchive
---

Please refer the full class documentation [here](QArchiveDiskCompressor.md)

This simple example compresses a set of files and directories.(Directories are recursively added)

## main.cpp

```
#include <QCoreApplication>
#include <QDebug>
#include <QArchive>

int main(int argc, char** argv)
{
    if(ac < 2) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [FILE(S)]";
        return 0;
    }
    using QArchive::DiskCompressor;    
    
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QStringList Files;
    
    for(auto i = 2; i < ac ; ++i){
            QString file = QString(av[i]);
            Files.append(file);
    }

    /* Construct DiskCompressor Object. */
    DiskCompressor Compressor(ArchivePath);
    Compressor.addFiles(Files);
    
    QObject::connect(&Compressor , &DiskCompressor::finished , [&](){
        qInfo() << "Compressed File(s) Successfully!";
        app.quit();
        return;
    });
    QObject::connect(&Compressor , &DiskCompressor::error , [&](short code , QString file){
        qInfo() << "An error has occured ::" 
		<< DiskCompressor::errorCodeToString(code) 
		<< "::" 
		<< file;
        app.quit();
        return;
    });

    Compressor.start();
    return app.exec();
}
```

## create_archive.pro

```
include(QArchive/QArchive.pri)
TEMPLATE = app
TARGET = create_archive
QT += core
SOURCES += main.cpp
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ qmake ..
 $ make -j$(nproc)
 $ ./create_archive
 $ # Make sure you have files for the new archive!
```

The Program is created in the examples tree on the official repo.

