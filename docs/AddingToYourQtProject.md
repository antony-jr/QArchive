---
id: AddingQArchive
title: Add QArchive to Your Qt Project
sidebar_label: Adding QArchive to Your Qt Project
---

## Using QArchive as library

Using **QArchive** as a library is always the best choice , Why ? Because if you add the    
source directly to your project , Everytime you use **QArchive** , The compiler compiles   
the whole library over and over. To avoid this , We compile **QArchive** once as a static
and a shared library and then include it in our Qt App.

To use **QArchive** as a library , You must use this directory structure.   
Other directory structures may work but this is the efficient one.

```
 -MyCoolApplication
  --libs
     ---QArchive
     ---libs.pro
  --src
     ---main.cpp
     ---mainwindow.cpp
     ---src.pro
  --MyCoolApplication.pro
```

### The Library Subdir Project file (libs.pro)

This is where you keep all third party libraries including **QArchive**.
Just add a **git submodule** or execute the steps mentioned in the **Installation**   
in the **libs** directory of your project folder.



```
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = QArchive
INCLUDEPATH += QArchive \
	       QArchive/include
```


### Your Source's Project file (src.pro)

```
TEMPLATE = app
TARGET = ../MyCoolApplication
QT += core concurrent gui # Everything else you need.
LIBS += -larchive -l../libs/QArchive/libQArchive.a # you can also use .so , its auto-generated.
INCLUDEPATH += . .. ../libs/QArchive \
               ../libs/QArchive/include
SOURCES += main.cpp mainwindow.cpp # All your source files.
# Add your header files too..
```

### Your Main Project file ( MyCoolApplication.pro )

```
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libs \ # Always use this order
	  src
INCLUDEPATH += . libs/QArchive \
               libs/QArchive/include
```

### Including QArchive in your Source

Whenever you want to use **QArchive** , you just need to include it!

```
#include <QArchive>
```


Thats it , All you have to do is to build your project with **qmake**.   
like this in your **Project Folder**.   

``` $ mkdir build; cd build ; qmake .. ; make -j$(nproc) ```


## Building QArchive with CMake

If you whish to build **QArchive** with CMake instead of qmake then you can do   
so by just adding the QArchive **directory** as a **sub-directory** in CMakeLists.txt.

refer the **CMake docs** for more information on building a Qt Project. For reference   
you can take a look at QArchive's **CMakeLists.txt**.


## Directly Adding QArchive to your Project

Since **QArchive** is a simple library you can also directly use it.

|	    |				               |		
|-----------|------------------------------------------|
|  Header:  | #include <QArchive>	               |
|   qmake:  | QT += core concurrent	       	       |
|   	    | LIBS += -larchive		               |
|	    | HEADERS += QArchive/include/QArchive.hpp |
|           | SOURCES += QArchive/src/QArchive.cc      |
| Inherits: | QObject			               |

**QArchive** is just a header and a source and all you have to do after installation is to add   
it in your **.pro** file ( **Qt Project file** ).

### Qt Project file (**.pro**)

Append these **lines** to your **Qt Project file**.

```
QT += core concurrent
LIBS += -larchive
HEADERS += QArchive/include/QArchive.hpp
SOURCES += QArchive/src/QArchive.cc
```

### Including QArchive in your Source

Whenever you want to use **QArchive** , you just need to include it!

```
#include <QArchive>
```

