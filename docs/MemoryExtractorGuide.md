---
id: MemoryExtractorGuide
title: Simple Extractor using QArchive (In Memory)
sidebar_label: Creating a Simple Extractor (In Memory)
---

Please refer the full class documentation [**here**.](QArchiveMemoryExtractor.md)

This simple example extracts a given archive (**in memory**) supported by libarchive. This examples make file expects that QArchive is installed to the system. (or you can also modify it to use add_subdirectory).


## main.cpp
```
#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
#include <QArchive/QArchive>

int main(int ac, char **av)
{
    if(ac == 1) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH]";
	return 0;
    }

    /* include the class from the namespace. */    
    using QArchive::MemoryExtractor;
    using QArchive::MemoryExtractorOutput;
    using QArchive::MemoryFile;

    QCoreApplication app(ac, av);

    /* Get Archive */
    QFile archive(QString(av[1]));
    if(!archive.open(QIODevice::ReadOnly)) {
	return -1;
    }

    /* Construct MemoryExtractor Object. */
    MemoryExtractor Extractor((QIODevice*)&archive);

    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Extractor , &MemoryExtractor::finished ,
    [&](MemoryExtractorOutput *output){
	qInfo() << "Extracted File(s) Successfully!";

	auto files  = output->getFiles();
	for(auto iter = files.begin(),
		 end = files.end();
		 end != iter;
		 ++iter) {
		auto fileInfo = (*iter).fileInformation();
		auto buffer = (*iter).buffer();

		// Display contents if it's a text file.
		qDebug() << "Filename:: " << fileInfo.value("FileName").toString();
		if(QFileInfo(fileInfo.value("FileName").toString()).suffix() == "txt") {
		   buffer->open(QIODevice::ReadOnly);
		   qDebug() << "Contents:: " << QString(buffer->readAll());
		   buffer->close();
		}
	}

	// Only delete this output QObject.
	output->deleteLater();
	app.quit();
	return;
    });
    QObject::connect(&Extractor , &MemoryExtractor::error ,
    [&](short code){
        qInfo() << "An error has occured ::"
                << QArchive::errorCodeToString(code);
        app.quit();
        return;
    });
    
    Extractor.start();
    return app.exec();
}
```

## CMakeLists.txt

```
CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
project(MemoryExtractor)
    
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
   
# Assuming you installed QArchive to the system.     
find_package(QArchive)
    
# Include Directories.
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})

add_executable(MemoryExtractor main.cpp)
target_link_libraries(MemoryExtractor PRIVATE QArchive)
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make -j$(nproc)
 $ ./MemoryExtractor
```

This Program is created in the examples tree on the official repo.

