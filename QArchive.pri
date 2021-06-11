INCLUDEPATH += $$PWD $$PWD/include $$PWD/other/qmake
QT += core
LIBS += -larchive
SOURCES += $$PWD/src/qarchivememoryextractor.cc \
	   $$PWD/src/qarchivediskextractor.cc \
	   $$PWD/src/qarchivediskcompressor_p.cc \
           $$PWD/src/qarchivediskcompressor.cc \
	   $$PWD/src/qarchiveextractor_p.cc \
           $$PWD/src/qarchiveutils_p.cc \
           $$PWD/src/qarchiveioreader_p.cc \
	   $$PWD/src/qarchive_enums.cc

HEADERS += $$PWD/include/qarchivediskcompressor_p.hpp \
	   $$PWD/include/qarchiveextractor_p.hpp \
	   $$PWD/include/qarchivememoryextractor.hpp \
           $$PWD/include/qarchiveutils_p.hpp \
           $$PWD/include/qarchiveioreader_p.hpp \
	   $$PWD/include/qarchive_enums.hpp \
           $$PWD/include/qarchivediskextractor.hpp \
           $$PWD/include/qarchivediskcompressor.hpp \
           $$PWD/include/qarchive_global.hpp \
           $$PWD/other/qmake/config.h
