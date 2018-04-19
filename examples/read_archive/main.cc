#include <QCoreApplication>
#include <QArchive>

int main(int ac, char **av)
{
    if(ac == 1) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH]";
        return 0;
    }
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QArchive::Reader Reader(ArchivePath); // Archive Path.
    Reader
    .setFunc(QArchive::STARTED, [&]() {
        qDebug() << "Starting Reader... ";
    })
    .setFunc(QArchive::FINISHED, [&]() {
        qDebug() << "Readed Entries Successfully!";
        app.quit();
        return;
    })
    .setFunc([&](QJsonObject files) {
        qDebug() << "Files:: ";
        qDebug() << files;
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
    QTimer::singleShot(100, &Reader, SLOT(start()));
    return app.exec();
}
