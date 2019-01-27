INCLUDEPATH += $$PWD $$PWD/include
QT += core
LIBS += -larchive
SOURCES += $$PWD/src/qarchivediskextractor_p.cc \
	   $$PWD/src/qarchivediskextractor.cc \
	   $$PWD/src/qarchivediskcompressor_p.cc \
           $$PWD/src/qarchivediskcompressor.cc \
	   $$PWD/src/qarchiveutils_p.cc \
	   $$PWD/src/qarchive_enums.cc

HEADERS += $$PWD/include/qarchivediskextractor_p.hpp \
	   $$PWD/include/qarchivediskcompressor_p.hpp \
           $$PWD/include/qarchiveutils_p.hpp \
	   $$PWD/include/qarchive_enums.hpp \
           $$PWD/include/qarchivediskextractor.hpp \
           $$PWD/include/qarchivediskcompressor.hpp
