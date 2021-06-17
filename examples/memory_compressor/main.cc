#include <QCoreApplication>
#include <QDebug>
#include <QFileInfo>
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

    using QArchive::MemoryCompressor;
    QCoreApplication app(ac, av);
    QString ArchivePath = QString(av[1]);
    QStringList Entries, Files;

    for(auto i = 2; i < ac ; ++i) {
        QString file = QString(av[i]);
        Entries.append(QFileInfo(file).fileName());
	Files.append(file);
    }

    /* Construct MemoryCompressor Object. */
    MemoryCompressor Compressor(QArchive::SevenZipFormat);
    Compressor.addFiles(Entries, Files);

    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Compressor, &MemoryCompressor::started, [&]() {
        qInfo() << "[+] Starting Compressor... ";
    });
    QObject::connect(&Compressor, &MemoryCompressor::finished, [&](QBuffer *buffer) {
        qInfo() << "[+] Compressed File(s) Successfully!";
        buffer->open(QIODevice::ReadOnly);
	QFile file(ArchivePath);
	file.open(QIODevice::WriteOnly);
	char buf[1024];
	size_t len = 0;
	len = buffer->read(buf, sizeof(buf));
	while(len > 0) {
		file.write(buf, len);
		len = buffer->read(buf, sizeof(buf));
	}
	file.close();
	buffer->close();
	buffer->deleteLater();
	app.quit();
        return;
    });
    QObject::connect(&Compressor, &MemoryCompressor::error, [&](short code, QString file) {
        qInfo() << "[-] An error has occured :: " << QArchive::errorCodeToString(code) << " :: " << file;

        app.quit();
        return;
    });

    QObject::connect(&Compressor, &MemoryCompressor::progress, [&](QString file,
    int proc, int total, qint64 br, qint64 bt) {
        qInfo() << "Progress::" << file << ":: Done ( " << proc << " / " << total << ") " << (br * 100/bt)  << "%.";
        return;
    });

    Compressor.start();
    return app.exec();
}
