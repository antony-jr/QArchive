#include <iostream>
#include <string>
#include <QCoreApplication>
#include <QDebug>
#ifdef BUILD_EXAMPLES
# include <QArchive>
#else
# include <QArchive/QArchive>
#endif
#include <QJsonObject>

int main(int ac, char **av) {
    if(ac < 2) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH] [DESTINATION PATH]";
        return 0;
    }

    using QArchive::MemoryExtractor;
    using QArchive::MemoryFile;
    using std::cout;
    using std::cin;
    using std::string;

    QCoreApplication app(ac, av);

    /* Get Archive Destination Information. */
    QString ArchivePath = QString(av[1]);
    QString DestinationPath = QString(av[2]);

    /* Construct MemoryExtractor Object. */
    MemoryExtractor Extractor(ArchivePath);
    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Extractor, &MemoryExtractor::extractionRequirePassword, [&](int tries) {
        string passwd;
        cout << "[*] Please Enter Archive Password(Tries = " << tries << "):: ";
        cin >> passwd;
        cout << "\n";

        /* Set the password. */
        Extractor.setPassword(QString::fromStdString(passwd));

        /* Again start the extractor. */
        Extractor.start();
    });

    QObject::connect(&Extractor, &MemoryExtractor::started, [&]() {
        qInfo() << "[+] Starting Extractor... ";
    });
    QObject::connect(&Extractor, &MemoryExtractor::finished,
    [&](QArchive::MemoryExtractorOutput *data) {
    	QVector<QArchive::MemoryFile> files = data->getFiles();

        qInfo() << "[+] Extracted File(s) Successfully! (In Memory)";
	
	for(auto iter = files.begin(),
		 end = files.end();
		 iter != end;
		 ++iter) {
		QJsonObject fileInfo = (*iter).fileInformation();
		QBuffer *buffer = (*iter).buffer(); 
		qDebug() << "FileName:: " << fileInfo.value("FileName").toString();
		qDebug() << "Buf Size:: " << buffer->size();
	}

	qInfo() << "Extracted " << files.count() << " File(s).";
	

	data->deleteLater();
	app.quit();
        return;
    });

    QObject::connect(&Extractor, &MemoryExtractor::error, [&](short code) {
        if(code == QArchive::ArchivePasswordNeeded ||
                code == QArchive::ArchivePasswordIncorrect) {
            return;
        }
        qInfo() << "[-] An error has occured :: " << QArchive::errorCodeToString(code) ;
        app.quit();
        return;
    });
    QObject::connect(&Extractor, &MemoryExtractor::info, [&](QJsonObject info) {
        qInfo() << "ARCHIVE CONTENTS:: " << info;
        return;
    });

    QObject::connect(&Extractor, &MemoryExtractor::progress, 
    [&](QString file, int proc, int total, qint64 br, qint64 bt) {
        qInfo() << "Progress("<< proc << "/" << total << "): "
                << file << " : " << (br*100/bt) << "% done.";
    });

    /* Start the Extractor.
     * Note:
     *    You don't really have to worry about the event loop since all method calls
     *    to the extractor object is queued and only gets executed when the event
     *    loop starts , most likely.
    */
    Extractor.setCalculateProgress(true);
    Extractor.getInfo();
    Extractor.start();
    return app.exec();
}
