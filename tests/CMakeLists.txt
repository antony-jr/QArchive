CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
project(QArchiveTests VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(APPLE)
   add_definitions(-DBUILD_TESTS)
endif()

if(NOT BUILD_TESTS)
	find_package(QArchive)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
if (NOT DEFINED QARCHIVE_QT_VERSION_MAJOR)
   set(QARCHIVE_QT_VERSION_MAJOR ${QT_VERSION_MAJOR})
endif()

if (QARCHIVE_QT_VERSION_MAJOR EQUAL 6)
   set(CMAKE_CXX_STANDARD 17)
else()
   set(CMAKE_CXX_STANDARD 11)
endif()

find_package(Qt${QARCHIVE_QT_VERSION_MAJOR} COMPONENTS Test Concurrent REQUIRED)

# Include Directories.

include_directories(${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME} main.cc
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
			  
target_link_libraries(${PROJECT_NAME} PRIVATE QArchive 
   Qt${QARCHIVE_QT_VERSION_MAJOR}::Test Qt${QARCHIVE_QT_VERSION_MAJOR}::Concurrent)

enable_testing()

add_test(
  NAME ${PROJECT_NAME}
  COMMAND $<TARGET_FILE:QArchiveTests>
  )

string(TOLOWER "${CMAKE_SYSTEM_NAME}" CMAKE_SYSTEM_NAME_LOWER)

if(CMAKE_SYSTEM_NAME_LOWER STREQUAL "windows")
    get_filename_component(QT_BIN_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
    set_tests_properties(
        ${PROJECT_NAME}
    PROPERTIES
        # Make sure DLL is found by adding its directory to PATH
        ENVIRONMENT "PATH=${QT_BIN_DIR}\;$<TARGET_PROPERTY:QArchive,BINARY_DIR>\;"
    )

endif()
