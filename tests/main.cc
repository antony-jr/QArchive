#include <QCoreApplication>
#include "QArchiveExtractorTests.hpp"
#include "QArchiveCompressorTests.hpp"

int main(int ac , char **av){
    QCoreApplication app(ac , av);
    QArchiveExtractor extractorTests;
    QArchiveCompressor compressorTests;
    if(QTest::qExec(&extractorTests)){
        return -1;
    }
    if(QTest::qExec(&compressorTests)){
        return -1;
    }
    QTimer::singleShot(1000 , &app , SLOT(quit()));
    return app.exec();
}
