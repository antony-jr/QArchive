#include <QCoreApplication>
#include <QArchive>

int main(int ac, char **av)
{
    if(ac == 1) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [DESTINATION PATH]";
        return 0;
    }
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QString DestinationPath = QString(av[2]);
    QArchive::Extractor Extractor(ArchivePath, DestinationPath);  // Archive Path.
    Extractor
    .setFunc(QArchive::STARTED, [&]() {
        qDebug() << "Starting Extractor... ";
    })
    .setFunc(QArchive::FINISHED, [&]() {
        qDebug() << "Extracted File(s) Successfully!";
        app.quit();
        return;
    })
    .setFunc([&](short code, QString eMsg) {
        qDebug() << "An error has occured :: error code -> " << code << " :: " << eMsg;
        app.quit();
        return;
    });
    /*
     * Note: If you start imediately , if the files to extract are very
     * small and needs only few miliseconds , QArchive Extractor will
     * finish it before you can start the event loop and so your program
     * never exits because the quit slot is shot before the event loop
     * is started.
    */
    QTimer::singleShot(100, &Extractor, SLOT(start()));
    return app.exec();
}
