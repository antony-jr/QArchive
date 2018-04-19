#include <QCoreApplication>
#include <QArchive>

int main(int ac, char **av)
{
    if(ac == 1) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [FILE(S)]";
        return 0;
    }
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QArchive::Compressor Compressor(ArchivePath); // Archive Path.
    Compressor
    .setFunc(QArchive::STARTED, [&]() {
        qDebug() << "Starting Compression... ";
    })
    .setFunc(QArchive::FINISHED, [&]() {
        qDebug() << "Compressed File(s) Successfully!";
        app.quit();
        return;
    })
    .setFunc([&](short code, QString eMsg) {
        qDebug() << "An error has occured :: error code -> " << code << " :: " << eMsg;
        app.quit();
        return;
    });
    for(int i = 2; i < ac ; ++i) {
        QString file = QString(av[i]);
        Compressor.addFiles(file);
    }
    /*
     * Note: If you start imediately , if the files to compress are very
     * small and needs only few miliseconds , QArchive Compressor will
     * finish it before you can start the event loop and so your program
     * never exits because the quit slot is shot before the event loop
     * is started.
    */
    QTimer::singleShot(100, &Compressor, SLOT(start()));
    return app.exec();
}
