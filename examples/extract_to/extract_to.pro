TEMPLATE = app
TARGET = extract_to
INCLUDEPATH += . ../../ ../../include
DEFINES += QT_DEPRECATED_WARNINGS

LIBS += -larchive
QT   += core concurrent

SOURCES += main.cpp ../../src/QArchive.cc
HEADERS += ../../include/QArchive.hpp

