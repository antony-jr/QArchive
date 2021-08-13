---
id: AddingQArchive
title: Add QArchive to Your Project
sidebar_label: Adding QArchive to Your Project
---

# Using QArchive with CMake

If you wish to build **QArchive** with CMake instead of qmake then you can do
so by just adding the QArchive **directory** as a **sub-directory** in CMakeLists.txt.

Use this directory structure or something similar. This structure is just a recommendation.

```
 -MyCoolApplication
  --libs
     ---QArchive
     ---CMakeLists.txt
  --main.cpp
  --mainwindow.cpp
  --CMakeLists.txt
```

### MyCoolApplication/libs/CMakeLists.txt

The contents of the CMakeLists should be something like below. To include QArchive repo with **add_subdirectory**.

```
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

add_subdirectory(QArchive)
```

### MyCoolApplication/CMakeLists.txt

Your main CMakeLists just need to use **add_subdirectory** on QArchive directory. Here we have
done **add_subdirectory** on lib directory which in turn add *QArchive*.


```
CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
project(MyCoolApplication)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(lib)
find_package(Qt5::Core)

add_executable(MyCoolApplication main.cc)
target_link_libraries(MyCoolApplication PUBLIC Qt::Core)
target_link_libraries(MyCoolApplication PRIVATE QArchive)
```

### Include it in your sources

```
#include <QArchive/QArchive>
```

### Building

```
 $ mkdir build
 $ cd build
 $ cmake .. && make -j$(nproc)
```

# Using QArchive with CMake (Conan) (**Pre-built Binaries**)

If you don't like to build **QArchive** from source, you can use **conan** to use QArchive pre-built binaries
directly in your program. The pre-built binaries are available for all major platforms. 

Use this directory structure or something similar.

```
 -MyCoolApplication
  --main.cpp
  --conanfile.py
  --CMakeLists.txt
```

### conanfile.py

This file describes conan package manager your dependencies.

```
from conans import ConanFile, CMake

class Conan(ConanFile):
   name = "MyCoolApplication"
   version = "1.0.0"
   settings = "arch"
   # By setting this this generator we instruct conan to generate the findPKG.cmake files.
   generators = "cmake_find_package"

   def configure(self):
      self.requires("qarchive/2.0.1") # Change this if you have to.
```

### CMakeLists.txt

Some conan magic needs to be included in the CMake file.

```
cmake_minimum_required(VERSION 3.15)
project(MyCoolApplication CXX)

# conan.cmake will be here
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# Conan-generated files will be here. We care about FindQArchive.cmake
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# Download conan.cmake module, used to automate conan calls.
# This could be replaced with manual conan calls before the cmake calls.
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

# Autodetect system settings so the example is crossplatform.
conan_cmake_autodetect(settings)

# Pull the QArchive and all its transitive dependencies. Since QArchive depends on Qt,
# there are a lot of transitive dependencies.
# This command also generate all the conan findPKG.cmake modules.
conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}/conanfile.py
                    REMOTE conan-center
                    SETTINGS ${settings}
)

find_package(QArchive REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# This is a requirement since we're using Conan's qt5
target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)

target_link_libraries(${PROJECT_NAME} QArchive::QArchive)
```

### Including it in your Sources

```
#include <QArchive/QArchive>
```

### Building

```
 $ mkdir build
 $ cd build
 $ cmake .. && make -j$(nproc)
```


# Using QArchive as library (QMake)

Using **QArchive** as a library is always the best choice, Why? Because if you add the
source directly to your project, Everytime you use **QArchive**, The compiler compiles
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

Whenever you want to use **QArchive**, you just need to include it!

```
#include <QArchive>
```


Thats it , All you have to do is to build your project with **qmake**.
like this in your **Project Folder**.   

``` $ mkdir build; cd build ; qmake .. ; make -j$(nproc) ```


# Directly Adding QArchive to your Project

You can also add **QArchive** directly to your project if you consider your project is small.

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

# DEFINES -= QARCHIVE_STATIC # uncomment this if you are building as shared library.
# LIBS += $$PWD/libarchive # uncomment to give custom dir to libarchive libs.
# INCLUDEPATH += $$PWD/libarchive-includes # uncomment to give custom dir to libarchive headers.
```

### Including QArchive in your Source

Whenever you want to use **QArchive** , you just need to include it!

```
#include <QArchive>
```

