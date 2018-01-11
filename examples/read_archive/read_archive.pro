TEMPLATE = app
TARGET = read_archive
INCLUDEPATH += . ../../
DEFINES += QT_DEPRECATED_WARNINGS

QT += core concurrent
LIBS += -larchive
SOURCES += main.cpp
HEADERS += ../../QArchive.hpp
