INCLUDEPATH += . ../ ../include/
QT += core concurrent testlib
LIBS += -larchive
SOURCES += main.cc ../src/QArchive.cc
HEADERS += QArchiveExtractorTests.hpp QArchiveCompressorTests.hpp ../include/QArchive.hpp
