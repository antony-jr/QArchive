#include <QCoreApplication>
#include <QArchiveDiskExtractorTests.hpp>

int main(int ac, char **av)
{
    QCoreApplication app(ac, av);
    QArchiveDiskExtractor extractorTests;
    if(QTest::qExec(&extractorTests)) {
        return -1;
    }
    QTimer::singleShot(1000, &app, SLOT(quit()));
    return app.exec();
}
