---
id: UsingReader
title: Using QArchive to Read Archives
sidebar_label: Reading Archives using QArchive.
---

Please refer the full class documentation [here](QArchiveReader.md)

This simple example reads an archive. 

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
    QArchive::Reader e("test.7z");


    /*
     * 2.Connect Callbacks
    */

    QObject::connect(&e, &QArchive::Reader::archiveFiles, [&](QString archive, QStringList files) {
        qDebug() << archive << " :: ";
        qDebug() << files;
        app.quit();
    });

    // emitted when something goes wrong
    QObject::connect(&e, &QArchive::Reader::error, [&](short code, QString file) {
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
     * 3.Start reading!
    */
    e.start();
    return app.exec();
}
```

## read_archive.pro

```
TEMPLATE = app
TARGET = read_archive

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
 $ ./read_archive
 $ # Make sure you have archive file!
```

Compile the above program by refering [here](AddingToYourQtProject.md)   
The Program is created in the examples tree on the official repo.

