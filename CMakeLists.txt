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
	    src/qarchivediskcompressor.cc
	    src/qarchivediskextractor.cc
	    src/qarchive_enums.cc
	    src/qarchivediskcompressor_p.cc
	    src/qarchivediskextractor_p.cc
	    src/qarchiveutils_p.cc
	    src/qarchiveioreader_p.cc
	    include/qarchivediskcompressor.hpp
	    include/qarchivediskextractor.hpp
	    include/qarchive_enums.hpp
	    include/qarchivediskcompressor_p.hpp
	    include/qarchivediskextractor_p.hpp
	    include/qarchiveutils_p.hpp
	    include/qarchiveioreader_p.hpp
	    )

target_link_libraries(QArchive PUBLIC Qt5::Core archive)
target_include_directories(QArchive PUBLIC . include)
