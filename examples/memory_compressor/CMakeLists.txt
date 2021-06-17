CMAKE_MINIMUM_REQUIRED(VERSION 3.17)
project(MemoryCompressor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(NOT BUILD_EXAMPLES)
	find_package(QArchive)
endif()

# Include Directories.
include_directories(.)
include_directories(${CMAKE_BINARY_DIR})

add_executable(MemoryCompressor main.cc)
target_link_libraries(MemoryCompressor PRIVATE QArchive)
