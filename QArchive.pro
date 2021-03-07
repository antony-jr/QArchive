# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# -------------------------------
#  QMake support for QArchive.
# -------------------------------
# Remove the below line if its not static build.
DEFINES += QARCHIVE_STATIC # if it is a static build.
include(QArchive.pri)
TEMPLATE = lib
CONFIG += static build_all release
TARGET = QArchive
DEFINES += QARCHIVE_BUILD QT_DEPRECATED_WARNINGS
