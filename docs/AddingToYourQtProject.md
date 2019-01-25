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
     ---helper.hpp
     ---src.pro
  --MyCoolApplication.pro
```

### The Library Subdir Project file (libs.pro)

This is where you keep all third party libraries including **QArchive**.
Just execute the steps mentioned in the **Installation** in the **libs**   
directory of your project folder.


```
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = QArchive
```


### Your Source's Project file (src.pro)

```
TEMPLATE = app
TARGET = ../MyCoolApplication
QT += core gui # Modules typically needed for any GUI App.

# Always include libs in this order , dont change.
LIBS += ../libs/QArchive/libQArchive.a -larchive 
INCLUDEPATH += . .. ../libs/QArchive \
               ../libs/QArchive/include
SOURCES += main.cpp mainwindow.cpp # All your source files.
HEADERS += helper.hpp # Add your header files.
```

### Your Main Project file ( MyCoolApplication.pro )

```
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libs \ # Always use this order
	  src
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

You can also add **QArchive** directly to your project if you consider your project is 
small.

|	    |				               |		
|-----------|------------------------------------------|
|  Header:  | #include < QArchive >	               |
|   qmake:  | include(QArchive/QArchive.pri)           |
| Inherits: | QObject			               |


You just need to include the **QArchive.pri** file in your project **.pro** file in order
to include everything needed for you to use it in your entire application. 
This method is less complicated and could be done for small projects but in large projects
this is very inefficient where the **QArchive source files** could be compiled multiple 
times.

### Qt Project file (**.pro**)

Prepend or append these **lines** to your **Qt Project file**.

```
include(QArchive/QArchive.pri)
```

### Including QArchive in your Source

Whenever you want to use **QArchive** , you just need to include it!

```
#include <QArchive>
```

