# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# -------------------------------
#  CMake Support for QArchive.
# -------------------------------

CMAKE_MINIMUM_REQUIRED( VERSION 3.2)
project(QArchive)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Find the QtCore library.
find_package(Qt5Core)

# Include Directories.
include_directories(.)
include_directories(include)

add_library(QArchive 
	    src/qarchivediskextractor_p.cc
	    src/qarchivediskextractor.cc
	    include/qarchivediskextractor_p.hpp
	    include/qarchivediskextractor.hpp
	    include/qarchive_enums.hpp)

target_link_libraries(QArchive PUBLIC Qt5::Core archive)
target_include_directories(QArchive PUBLIC . include)
