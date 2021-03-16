---
id: ReportingProgressGuide
title: Reporting Progress for Extraction
sidebar_label: Reporting Progress
---

Please refer the full class documentation [here](QArchiveDiskExtractor.md)

This simple example extracts a given archive and also reports the progress.
Eventhough this example is for the extractor , the same can be used for 
**compressor** too.

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
    
    QObject::connect(&Extractor , &DiskExtractor::progress ,
    [&](QString file , int entriesProcessed , int entriesTotal , qint64 bytesProcessed, qint64 bytesTotal){
	qInfo() << "Progress::" 
                << file 
                << ":: Entry ( " 
                << entriesProcessed 
                << " / " 
                << entriesTotal 
                << ") " 
                << float(bytesProcessed) * 100 / float(bytesTotal)
                << "%.";
	return;
    });

    QObject::connect(&Extractor , &DiskExtractor::error ,
    [&](short code){
        qInfo() << "An error has occured ::"
                << QArchive::errorCodeToString(code);
        app.quit();
        return;
    });

    /* By default this is false. */
    Extractor.setCalculateProgress(true); 
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

