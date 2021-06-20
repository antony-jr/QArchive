---
id: CreatingEncryptedZipArchiveGuide
title: Creating Encrypted ZIP Archive using QArchive
sidebar_label: Creating Encrypted Archive
---

Please refer the full class documentation [**here**.](QArchiveDiskCompressor.md)

This simple example compresses a set of files and directories(Directories are recursively added) and
encrypt the archive using a password.

**Please see that only Zip format is supported by libarchive for encryption and thus if you 
try to encrypt any other formats , it will be ignored.**


## main.cpp

```
#include <QCoreApplication>
#include <QDebug>
#include <QArchive/QArchive>

int main(int argc, char** argv)
{
    if(ac < 2) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [FILE(S)]";
        return 0;
    }
    using QArchive::DiskCompressor;    
    
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QStringList Files;
    
    for(auto i = 2; i < ac ; ++i){
            QString file = QString(av[i]);
            Files.append(file);
    }

    /* Construct DiskCompressor Object. */
    DiskCompressor Compressor(ArchivePath);
    Compressor.addFiles(Files);
    Compressor.setArchiveFormat(QArchive::ZipFormat);
    Compressor.setPassword("YOUR PASSWORD HERE");
    
    QObject::connect(&Compressor , &DiskCompressor::finished , [&](){
        qInfo() << "Compressed File(s) Successfully!";
        app.quit();
        return;
    });
    QObject::connect(&Compressor , &DiskCompressor::error ,
    [&](short code , QString file){
        qInfo() << "An error has occured ::" 
		<< QArchive::errorCodeToString(code) 
		<< "::" 
		<< file;
        app.quit();
        return;
    });
    Compressor.start();
    return app.exec();
}
```

## create_archive.pro

```
CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
project(DiskCompressor)
    
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Assuming you installed QArchive to system.        
find_package(QArchive)
    
# Include Directories.
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})

add_executable(DiskCompressor main.cpp)
target_link_libraries(DiskCompressor PRIVATE QArchive)
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ qmake ..
 $ make -j$(nproc)
 $ ./DiskCompressor
```
