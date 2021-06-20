---
id: ExtractingPasswordProtectedArchiveGuide
title: Extracting encrypted Archives using QArchive
sidebar_label: Extracting Encrypted Archives
---

Please refer the full class documentation [**here**.](QArchiveDiskExtractor.md)

This simple example extracts a given archive which is encrypted.

## main.cpp
```
#include <iostream>
#include <string>
#include <QCoreApplication>
#include <QDebug>
#include <QArchive/QArchive>

int main(int ac, char **av)
{
    if(ac < 2) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [DESTINATION PATH]";
        return 0;
    }

    /* include the class from the namespace. */    
    using QArchive::DiskExtractor;
    using std::cout;
    using std::cin;
    using std::string;

    QCoreApplication app(ac, av);

    /* Get Archive Destination Information. */
    QString ArchivePath = QString(av[1]); 
    QString DestinationPath = QString(av[2]);

    /* Construct DiskExtractor Object. */
    DiskExtractor Extractor(ArchivePath , DestinationPath);
    
    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Extractor , &DiskExtractor::finished , [&](){
        qInfo() << "Extracted File(s) Successfully!";
        app.quit();
        return;
    });

    QObject::connect(&Extractor , &DiskExtractor::extractionRequirePassword , 
    [&](int tries){
	string passwd;
	cout << "[*] Please Enter Archive Password(Tries = " << tries << "):: ";
	cin >> passwd;
	cout << "\n";

	/* Set the password. */
	Extractor.setPassword(QString::fromStdString(passwd));

	/* Again start the extractor. */
	Extractor.start();	
    });

    QObject::connect(&Extractor , &DiskExtractor::error ,
    [&](short code){
        /* Avoid password errors */
        if(code == QArchive::ArchivePasswordNeeded || 
	   code == QArchive::ArchivePasswordIncorrect){
		(void)archive;
		return;
	}	
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
project(DiskExtractor)
    
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Assuming you installed QArchive to system.        
find_package(QArchive)
    
# Include Directories.
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})

add_executable(DiskExtractor main.cpp)
target_link_libraries(DiskExtractor PRIVATE QArchive)
```

## Compilation and Execution

```
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make -j$(nproc)
 $ ./DiskExtractor
```

This Program is created in the examples tree on the official repo.

