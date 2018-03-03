# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# -------------------------------
#  QMake support for QArchive.
# -------------------------------

TEMPLATE = lib
CONFIG += shared_and_static build_all release
TARGET = QArchive
INCLUDEPATH += . include
DEFINES += QT_DEPRECATED_WARNINGS

QT += core concurrent 
LIBS += -larchive

# Inputs 
SOURCES += src/QArchive.cc
HEADERS += include/QArchive.hpp
