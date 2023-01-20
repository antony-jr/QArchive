# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# -------------------------------
#  CMake Support for QArchive.
# -------------------------------
# OPTIONS:
#    -DBUILD_TESTS=ON -> Build tests for library, defaults to OFF.
#    -DBUILD_SHARED_LIBS=ON -> Shared libs, defaults to OFF
#    -DQARCHIVE_QT_VERSION_MAJOR=6 -> Use this major version, defaults to 6
#				      if found or else 5
# ---

CMAKE_MINIMUM_REQUIRED( VERSION 3.17)
project(QArchive VERSION 2.2.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Note to other developers,
# These are privately maintained options, not for
# public use. One should not set these manually from
# cmake build.
option(QARCHIVE_STATIC "Static Build of QArchive" ON)
# ---

# Find the QtCore library and libarchive.
# In macOS, if libarchive is installed via homebrew,
# it does not link well with CMake. The only workaround to
# this is to give the absolute path of the libarchive pkg
# config (.pc) file to find the actual libarchive installed
# via brew. If this is not done, cmake always seems to find 
# the libarchive installed in system by default.
# 
# Usage:
#  cmake -DLIBARCHIVE_PKG_CONFIG=$(brew --prefix libarchive)/lib/pkgconfig/
# ---
if(APPLE AND LIBARCHIVE_PKG_CONFIG)
   set(ENV{PKG_CONFIG_PATH} "${LIBARCHIVE_PKG_CONFIG}")
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(LIBARCHIVE REQUIRED libarchive IMPORTED_TARGET)
   if(NOT LIBARCHIVE_FOUND)
      find_package(LibArchive REQUIRED)
   endif()
else ()
   find_package(LibArchive REQUIRED)
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
if (NOT DEFINED QARCHIVE_QT_VERSION_MAJOR)
   set(QARCHIVE_QT_VERSION_MAJOR ${QT_VERSION_MAJOR})
endif()

# Qt 6.4.x requires C++ 17 standard, so let's add an exception 
# for that,  also we should have started testing this.
# I suppose Ubuntu repo has a old Qt6 version.
if (QARCHIVE_QT_VERSION_MAJOR EQUAL 6)
   set(CMAKE_CXX_STANDARD 17)
else()
   set(CMAKE_CXX_STANDARD 11)
endif()

find_package(Qt${QARCHIVE_QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

# cmake macros used
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Include Directories.
include_directories(.)
include_directories(include)

# Required for the import and exports 
# for Windows Builds.
if(BUILD_SHARED_LIBS)
	set(QARCHIVE_STATIC OFF)
	add_definitions(-DQARCHIVE_BUILD)
endif()


if(BUILD_TESTS)
	add_definitions(-DBUILD_TESTS)
	add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
	add_definitions(-DBUILD_EXAMPLES)
	add_subdirectory(examples/disk_compressor)
	add_subdirectory(examples/disk_extractor)
	add_subdirectory(examples/disk_extractor_with_QIODevice)
	add_subdirectory(examples/memory_extractor)
	add_subdirectory(examples/memory_compressor)
endif()

add_library(QArchive
	    src/qarchive_enums.cc
	    src/qarchiveutils_p.cc
	    src/qarchivememoryfile.cc
	    src/qarchivememoryextractoroutput.cc
	    src/qarchiveioreader_p.cc
	    src/qarchiveextractor_p.cc
	    src/qarchiveextractor.cc
	    src/qarchivecompressor_p.cc
	    src/qarchivecompressor.cc
	    src/qarchivediskextractor.cc
	    src/qarchivememoryextractor.cc
	    src/qarchivediskcompressor.cc
	    src/qarchivememorycompressor.cc
	    include/qarchive_enums.hpp
	    include/qarchiveutils_p.hpp
	    include/qarchivememoryfile.hpp
	    include/qarchivememoryextractoroutput.hpp
	    include/qarchiveioreader_p.hpp
	    include/qarchiveextractor_p.hpp
	    include/qarchiveextractor.hpp
	    include/qarchivecompressor_p.hpp
	    include/qarchivecompressor.hpp
	    include/qarchivediskextractor.hpp
	    include/qarchivememoryextractor.hpp
	    include/qarchivediskcompressor.hpp
	    include/qarchivememorycompressor.hpp
	    include/qarchive_global.hpp)

SET(toinstall)
list(APPEND toinstall
    QArchive
    include/qarchive_enums.hpp
    include/qarchivememoryfile.hpp
    include/qarchivememoryextractoroutput.hpp
    include/qarchiveextractor.hpp
    include/qarchivecompressor.hpp
    include/qarchivediskextractor.hpp
    include/qarchivememoryextractor.hpp
    include/qarchivediskcompressor.hpp
    include/qarchivememorycompressor.hpp
    include/qarchive_global.hpp
    ${PROJECT_BINARY_DIR}/config.h
)	

if (APPLE AND LIBARCHIVE_FOUND)
   target_link_libraries(QArchive PUBLIC Qt${QARCHIVE_QT_VERSION_MAJOR}::Core PkgConfig::LIBARCHIVE)
else()
   target_link_libraries(QArchive PUBLIC Qt${QARCHIVE_QT_VERSION_MAJOR}::Core LibArchive::LibArchive)
endif()

target_include_directories(QArchive INTERFACE
				$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
				$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
				$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)
target_include_directories(QArchive INTERFACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QArchive>" )

# create a config.h to decide the type of build
# when including public headers.
configure_file(
	"${PROJECT_SOURCE_DIR}/other/cmake/config.h.in"
	"${PROJECT_BINARY_DIR}/config.h"
	 @ONLY
)

# Add pkg-config and install instructions
configure_file(
	"${PROJECT_SOURCE_DIR}/other/pkgconfig/QArchive.pc.in"
	"${PROJECT_BINARY_DIR}/QArchive.pc"
  @ONLY
)

install(FILES
	${PROJECT_BINARY_DIR}/QArchive.pc
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

install(TARGETS
	QArchive
	EXPORT QArchiveTargets
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES
  ${toinstall}
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/QArchive")

# Add CMake config
set(CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/QArchive")
configure_package_config_file(
	"${PROJECT_SOURCE_DIR}/other/cmake/QArchiveConfig.cmake.in"
	"${CMAKE_CURRENT_BINARY_DIR}/QArchiveConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/QArchiveConfig.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}")

install(EXPORT QArchiveTargets
	FILE QArchiveTargets.cmake
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}")
