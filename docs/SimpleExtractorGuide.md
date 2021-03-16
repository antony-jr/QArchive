---
id: SimpleExtractorGuide
title: Simple Extractor using QArchive
sidebar_label: Creating a Simple Extractor
---

Please refer the full class documentation [here](QArchiveDiskExtractor.md)

This simple example extracts a given archive supported by libarchive.

## main.cpp
```
#include <QCoreApplication>
#include <QDebug>
#include <QArchive/QArchive>

int main(int ac, char **av)
{
    if(ac < 2) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [DESTINATION PATH]";
        return 0;
    }

    /* include the class from the namespace. */    
    using QArchive::DiskExtractor;

    QCoreApplication app(ac, av);

    /* Get Archive Destination Information. */
    QString ArchivePath = QString(av[1]); 
    QString DestinationPath = QString(av[2]);

    /* Construct DiskExtractor Object. */
    DiskExtractor Extractor(ArchivePath , DestinationPath);
    
    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Extractor , &DiskExtractor::finished , [&](){
        qInfo() << "Extracted File(s) Successfully!";
        app.quit();
        return;
    });
    QObject::connect(&Extractor , &DiskExtractor::error ,
    [&](short code){
        qInfo() << "An error has occured ::"
                << QArchive::errorCodeToString(code);
        app.quit();
        return;
    });
    
    Extractor.start();
    return app.exec();
}
```

## extract.pro

```
include(QArchive/QArchive.pri)
TEMPLATE = app
TARGET = extract
SOURCES += main.cpp
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ qmake ..
 $ make -j$(nproc)
 $ ./extract
```

This Program is created in the examples tree on the official repo.

