#include <QCoreApplication>
#include "QArchiveExtractorTests.hpp"
#include "QArchiveCompressorTests.hpp"
#include "QArchiveReaderTests.hpp"

int main(int ac, char **av)
{
    QCoreApplication app(ac, av);
    QArchiveExtractor extractorTests;
    QArchiveCompressor compressorTests;
    QArchiveReader readerTests;
    if(QTest::qExec(&extractorTests)) {
        return -1;
    }
    if(QTest::qExec(&compressorTests)) {
        return -1;
    }
    if(QTest::qExec(&readerTests)){
        return -1;
    }
    QTimer::singleShot(1000, &app, SLOT(quit()));
    return app.exec();
}
