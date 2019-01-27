# BSD-3 Clause.
# Copyright (C) 2018 Antony Jr.
# -------------------------------
#  QMake support for QArchive.
# -------------------------------
include(QArchive.pri)
TEMPLATE = lib
CONFIG += shared_and_static build_all release
TARGET = QArchive
DEFINES += QT_DEPRECATED_WARNINGS
