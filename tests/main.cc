#include <QCoreApplication>
#include <QArchiveExtractorTests.hpp>

int main(int ac, char **av)
{
    QCoreApplication app(ac, av);
    QArchiveExtractor extractorTests;
    if(QTest::qExec(&extractorTests)) {
        return -1;
    }
    QTimer::singleShot(1000, &app, SLOT(quit()));
    return app.exec();
}
