---
id: UsingCompressor
title: Using QArchive to Compress a set of files
sidebar_label: Compressing files with QArchive
---

Please refer the full class documentation [here](QArchiveCompressor.md)

This simple example compresses a set of files and thus creates an archive!

## main.cpp

```
#include <QCoreApplication>
#include <QDebug>
#include <QArchive>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    /*
     * 1.Construct
    */
    QArchive::Compressor e("test.7z", "TestDir");
    //			Archive		|--> Can also
    //			can be		     simply add
    //			detected	     directories.
    //			with extension.

    /*
     * 2.Connect Callbacks
    */

    // emitted when all extraction is finished
    QObject::connect(&e, &QArchive::Compressor::finished, [&]() {
        qDebug() << "Finished all jobs";
        app.quit();
    });
    QObject::connect(&e, &QArchive::Compressor::error, [&](short code, QString file) {
        qDebug() << "error code:: " << code << " :: " << file;
        app.quit();
    });
    /*
     * 3.Start compression!
    */
    e.start();

    qDebug() << "Its Non-Blocking!";

    return app.exec();
}
```

## create_archive.pro

```
TEMPLATE = app
TARGET = create_archive

QT += core concurrent
SOURCES += main.cpp
LIBS += -larchive
HEADERS += QArchive/include/QArchive.hpp
SOURCES += QArchive/src/QArchive.cc
INCLUDEPATH += QArchive/include \
	       QArchive
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ qmake ..
 $ make -j4
 $ ./create_archive
 $ # Make sure you have files for the new archive!
```

Compile the above program by refering [here](AddingToYourQtProject.md)   
The Program is created in the examples tree on the official repo.

