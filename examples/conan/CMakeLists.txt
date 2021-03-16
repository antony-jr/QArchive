cmake_minimum_required(VERSION 3.15)
project(quarchiveProject CXX)

# conan.cmake will be here
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# Conan-generated files will be here. We care about FindQArchive.cmake
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

# Download conan.cmake module, used to automate conan calls.
# This could be replaced with manual conan calls before the cmake calls.
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

# Autodetect system settings so the example is crossplatform.
conan_cmake_autodetect(settings)

# Pull the QArchive and all its transitive dependencies. Since QArchive depends on Qt,
# there are a lot of transitive dependencies.
# This command also generate all the conan findPKG.cmake modules.
conan_cmake_install(PATH_OR_REFERENCE ${CMAKE_SOURCE_DIR}/conanfile.py
                    REMOTE conan-center
                    SETTINGS ${settings}
)

find_package(QArchive REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# This is a requirement since we're using Conan's qt5
target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)

target_link_libraries(${PROJECT_NAME} QArchive::QArchive)

