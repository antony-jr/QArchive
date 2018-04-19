---
id: UsingExtractor
title: Using QArchive to Extract Archives
sidebar_label: Extracting Archives with QArchive
---

Please refer the full class documentation [here](QArchiveExtractor.md)

This simple example extracts an archive.
Here in this example you can see that we use a strange method called **'setFunc'** ,   
This method is little special, It connects **lambda** functions with the desired   
signal of the **extractor class**. This method was created to make this easy for   
you. You can also use **QObject::connect** to connect with signals in the   
**extractor class** like tradition qt libraries does. See [here](QArchiveSignalCodes.md) to get the signal
codes for your desired signals.

**Note** : All methods returns the reference to your current object and therefore you
can use other methods right away , Except for **isXXX()** methods because they return
bools. This feature is new to QArchive (to *The best of my knowledge*). This simply
means that you can extract a archive with a single line like this.

```
 QArchive::Extractor("test.7z").start().waitForFinished();
```

**Side Note** : Your event loop will not **quit** if you finish the archive process
before you start your event loop. That is , If you extract a very small archive , It 
will be finished before the next line gets executed ( *Yeah , Its really fast.* )   
and therefore your program never ends , One way to solve this is to use   
**QTimer::singleShot** to start the extractor with a **1000 miliseconds** delay.

## main.cpp
```
#include <QCoreApplication>
#include <QArchive>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QArchive::Extractor("test.7z")
    .setFunc(QArchive::FINISHED, [&]() {
        qDebug() << "Finished all extraction!";
        app.quit();
    })
    .start();
    qDebug() << "Its Non-Blocking!";
    return app.exec();
}
```

## extraction.pro

```
TEMPLATE = app
TARGET = extraction

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
 $ ./extraction
 $ # Make sure you have test.7z file!
```

Compile the above program by refering [here](AddingToYourQtProject.md)   
The Program is created in the examples tree on the official repo.

