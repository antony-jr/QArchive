TEMPLATE = app
TARGET = extract_interrupt
INCLUDEPATH += . ../../
DEFINES += QT_DEPRECATED_WARNINGS

QT += core
LIBS += -larchive
SOURCES += main.cpp
HEADERS += ../../QArchive.hpp
