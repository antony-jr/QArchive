CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
project(QArchiveTests VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(NOT BUILD_TESTS)
	find_package(QArchive)
endif()
find_package(Qt5Test)
find_package(Qt5Concurrent)

# Include Directories.
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})

add_executable(QArchiveTests main.cc
	                     TestRunner.cc
			     TestRunner.hpp
			     QArchiveTestCases.hpp
			     QArchiveDiskCompressorTests.cc
			     QArchiveDiskCompressorTests.hpp
			     QArchiveMemoryCompressorTests.cc
			     QArchiveMemoryCompressorTests.hpp
			     QArchiveDiskExtractorTests.cc
			     QArchiveDiskExtractorTests.hpp
			     QArchiveMemoryExtractorTests.cc
			     QArchiveMemoryExtractorTests.hpp)
target_link_libraries(QArchiveTests PRIVATE QArchive Qt5::Test Qt5::Concurrent)
