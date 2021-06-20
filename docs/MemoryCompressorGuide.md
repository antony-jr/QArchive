---
id: MemoryCompressorGuide
title: Simple Compressor using QArchive (In Memory)
sidebar_label: Creating Simple Compressor (In Memory)
---

Please refer the full class documentation [**here**](QArchiveMemoryCompressor.md).

This simple example compresses a set of files and directories **within memory**.(Directories are recursively added)

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
    using QArchive::MemoryCompressor;    
    
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QStringList Files;
    
    for(auto i = 2; i < ac ; ++i){
            QString file = QString(av[i]);
            Files.append(file);
    }

    /* Construct MemoryCompressor Object. */
    MemoryCompressor Compressor(QArchive::SevenZipFormat);
    Compressor.addFiles(Files);
    
    QObject::connect(&Compressor , &MemoryCompressor::finished , [&](QBuffer *archive){
	qInfo() << "Compressed File(s) Successfully!";
	qDebug() << "Compressed Archive Size:: " << archive->size();
	
	QFile out(ArchivePath);
	archive->open(QIODevice::ReadOnly);
	if(out.open(QIODevice::WriteOnly)) {
		char buff[1024];
		auto len = archive->read(buff, sizeof(buff));
		while (len > 0) {
			file.write(buff, len);
			len = archive->read(buff, sizeof(buff));
		}
	}
	archive->close();
	out.close();
 
	archive->deleteLater();
	app.quit();
	return;
    });
    QObject::connect(&Compressor , &MemoryCompressor::error ,
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

## CMakeLists.txt

```
CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
project(MemoryCompressor)
    
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Assuming you installed QArchive to system.        
find_package(QArchive)
    
# Include Directories.
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})

add_executable(MemoryCompressor main.cpp)
target_link_libraries(MemoryCompressor PRIVATE QArchive)
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make -j$(nproc)
 $ ./MemoryCompressor
```

This Program is created in the examples tree on the official repo.

