---
id: UsingExtractor
title: Using QArchive to Extract Archives
sidebar_label: Extracting Archives with QArchive
---

Please refer the full class documentation [here](QArchiveExtractor.md)

This simple example extracts an archive.


## main.cpp

```
#include <QCoreApplication>
#include <QDebug>
#include "QArchive/QArchive.hpp"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    /*
     * 1.Construct
    */
    QArchive::Extractor e("test.7z");


    /*
     * 2.Connect Callbacks
    */

    // emitted when all extraction is finished
    QObject::connect(&e, &QArchive::Extractor::finished, [&]() {
        qDebug() << "Finished all extraction!";
        app.quit();
    });

    QObject::connect(&e, &QArchive::Extractor::extracting, [&](QString file) {
        qDebug() << "Extracting:: " << file;
    });

    // emitted when a file is extracted
    QObject::connect(&e, &QArchive::Extractor::extracted, [&](QString file) {
        qDebug() << "Extracted:: " << file;
    });

    // emitted when something goes wrong
    QObject::connect(&e, &QArchive::Extractor::error, [&](short code, QString file) {
        switch(code) {
        case QArchive::ARCHIVE_READ_ERROR:
            qDebug() << "unable to find archive :: " << file;
            app.quit();
            break;
        case QArchive::ARCHIVE_QUALITY_ERROR:
            qDebug() << "bad archive! :: " << file;
            app.quit();
            break;
        case QArchive::ARCHIVE_UNCAUGHT_ERROR:
            qDebug() << "fatal error. :: " << file;
            app.quit();
            break;
        default:
            qDebug() << "unknown error. :: " << file;
            app.quit();
            break;
        }
    });

    /*
     * 3.Start extraction!
    */
    e.start();

    qDebug() << "Its Non-Blocking!";

    return app.exec();
}
```

## extraction.pro

```
TEMPLATE = app
TARGET = extraction

QT += core concurrent
LIBS += -larchive
SOURCES += main.cpp
HEADERS += QArchive/QArchive.hpp
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ qmake ..
 $ make -j4
 $ ./extraction
 $ # Make sure you have test.7z file!
```

Compile the above program by refering [here](AddingToYourQtProject.md)   
The Program is created in the examples tree on the official repo.

