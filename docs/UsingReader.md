---
id: UsingReader
title: Using QArchive to Read Archives
sidebar_label: Reading Archives using QArchive.
---

Please refer the full class documentation [here](QArchiveReader.md)

This simple example reads an archive. 
Here in this example you can see that we use a strange method called **'setFunc'** ,   
This method is little special, It connects **lambda** functions with the desired   
signal of the **reader class**. This method was created to make this easy for   
you. You can also use **QObject::connect** to connect with signals in the   
**reader class** like tradition qt libraries does. See [here](QArchiveSignalCodes.md) to get the signal
codes for your desired signals.

**Note** : All methods returns the reference to your current object and therefore you
can use other methods right away , Except for **isXXX()** methods because they return
bools. This feature is new to QArchive (to *The best of my knowledge*). This simply
means that you can read a archive with a single line like this.

```
  qDebug() << QArchive::Reader("Test.7z").start().waitForFinished().getFilesList();
```

**Side Note** : Your event loop will not **quit** if you finish the archive process
before you start your event loop. That is , If you read a very small archive , It 
will be finished before the next line gets executed ( *Yeah , Its really fast.* )   
and therefore your program never ends , One way to solve this is to use   
**QTimer::singleShot** to start the reader with a **1000 miliseconds** delay.

**IMPORTANT** : Do not use the class without explicit declaration.

## main.cpp

```
#include <QCoreApplication>
#include <QArchive>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    // Do not use: QArchive::Reader("test.7z")
    // when using setFunc as signal and slots
    // has some limitations.

    QArchive::Reader Reader("test.7z");

    Reader
    .setFunc([&](QJsonObject files){
        qDebug() << files;
        app.quit();
    })
    .start();
    qDebug() << "Its Non-Blocking!";
    return app.exec();
}
```

## read_archive.pro

```
TEMPLATE = app
TARGET = read_archive

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
 $ ./read_archive
 $ # Make sure you have archive file!
```

Compile the above program by refering [here](AddingToYourQtProject.md)   
The Program is created in the examples tree on the official repo.

