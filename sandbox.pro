TEMPLATE = app
CONFIG += debug
TARGET = sandbox
INCLUDEPATH += . include
DEFINES += QT_DEPRECATED_WARNINGS

QT += core
LIBS += -larchive

# Inputs 
SOURCES += src/qarchivediskextractor_p.cc sandbox.cc
HEADERS += include/qarchivediskextractor_p.hpp
