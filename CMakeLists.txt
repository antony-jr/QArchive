# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr. , All Rights Reserved.
CMAKE_MINIMUM_REQUIRED( VERSION 3.2)

project(QArchive)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Find the QtCore library
find_package(Qt5Core)
find_package(Qt5Concurrent)
find_package(archive)
include_directories(.)
include_directories(include)

add_library(QArchive src/QArchive.cc include/QArchive.hpp)
target_link_libraries(QArchive PUBLIC Qt5::Core Qt5::Concurrent archive)
target_include_directories(QArchive PUBLIC . include)
