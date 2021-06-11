#include <QCoreApplication>
#include <QDebug>
#ifdef BUILD_EXAMPLES
# include <QArchive>
#else
# include <QArchive/QArchive>
#endif 

int main(int ac, char **av) {
    if(ac < 2) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [FILE(S)]";
        return 0;
    }

    using QArchive::DiskCompressor;
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QStringList Files;

    for(auto i = 2; i < ac ; ++i) {
        QString file = QString(av[i]);
        Files.append(file);
    }

    /* Construct DiskCompressor Object. */
    DiskCompressor Compressor(ArchivePath);
    Compressor.addFiles(Files);

    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Compressor, &DiskCompressor::started, [&]() {
        qInfo() << "[+] Starting Compressor... ";
    });
    QObject::connect(&Compressor, &DiskCompressor::finished, [&]() {
        qInfo() << "[+] Compressed File(s) Successfully!";
        app.quit();
        return;
    });
    QObject::connect(&Compressor, &DiskCompressor::error, [&](short code, QString file) {
        qInfo() << "[-] An error has occured :: " << QArchive::errorCodeToString(code) << " :: " << file;

        app.quit();
        return;
    });

    QObject::connect(&Compressor, &DiskCompressor::progress, [&](QString file,
    int proc, int total, qint64 br, qint64 bt) {
        qInfo() << "Progress::" << file << ":: Done ( " << proc << " / " << total << ") " << (br * 100/bt)  << "%.";
        return;
    });

    /* Start the Compressor.
     * Note:
     *    You don't really have to worry about the event loop since all method calls
     *    to the extractor object is queued and only gets executed when the event
     *    loop starts , most likely.
    */
    Compressor.start();
    return app.exec();
}
