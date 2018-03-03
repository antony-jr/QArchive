TEMPLATE = app
TARGET = extraction
INCLUDEPATH += . ../../ ../../include/
DEFINES += QT_DEPRECATED_WARNINGS

QT += core concurrent
LIBS += -larchive
SOURCES += main.cpp ../../src/QArchive.cc
HEADERS += ../../include/QArchive.hpp
