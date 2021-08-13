# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# -------------------------------
#  QMake support for QArchive.
# -------------------------------
# Remove the below line if its not static build.
include(QArchive.pri)
TEMPLATE = lib
CONFIG += static build_all release
# DEFINES -= QARCHIVE_STATIC # uncomment if you compile as shared library.
# LIBS += $$PWD/libarchive # uncomment to give custom dir to libarchive libs.
# INCLUDEPATH += $$PWD/libarchive-includes # uncomment to give custom dir to libarchive headers.
TARGET = QArchive
DEFINES += QARCHIVE_BUILD QT_DEPRECATED_WARNINGS
