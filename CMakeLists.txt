# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# -------------------------------
#  CMake Support for QArchive.
# -------------------------------

CMAKE_MINIMUM_REQUIRED( VERSION 3.17)
project(QArchive VERSION 2.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


# Find the QtCore library and libarchive.
find_package(LibArchive)
find_package(Qt5Core)

# cmake macros used
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Include Directories.
include_directories(.)
include_directories(include)

if(BUILD_EXAMPLES)
	add_subdirectory(examples/disk_compressor)
	add_subdirectory(examples/disk_extractor)
	add_subdirectory(examples/disk_extractor_with_QIODevice)
endif()

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

SET(toinstall)
list(APPEND toinstall
    QArchive
    include/qarchive_enums.hpp
    include/qarchivediskextractor.hpp
    include/qarchivediskcompressor.hpp
)	

target_link_libraries(QArchive PUBLIC Qt5::Core LibArchive::LibArchive)
target_include_directories(QArchive INTERFACE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include>")
target_include_directories(QArchive INTERFACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/QArchive>" )

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
  DESTINATION "${CMAKE_INSTALL_LIBDIR}")

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
