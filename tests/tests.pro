INCLUDEPATH += . ../ ../include/
QT += core concurrent testlib
LIBS += -larchive
SOURCES += ../src/QArchive.cc
HEADERS += QArchiveExtractorTests.hpp ../include/QArchive.hpp
