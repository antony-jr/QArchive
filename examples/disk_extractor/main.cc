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

    using QArchive::DiskExtractor;
    using std::cout;
    using std::cin;
    using std::string;

    QCoreApplication app(ac, av);

    /* Get Archive Destination Information. */
    QString ArchivePath = QString(av[1]);
    QString DestinationPath = QString(av[2]);

    /* Construct DiskExtractor Object. */
    DiskExtractor Extractor(ArchivePath, DestinationPath);
    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Extractor, &DiskExtractor::extractionRequirePassword, [&](int tries) {
        string passwd;
        cout << "[*] Please Enter Archive Password(Tries = " << tries << "):: ";
        cin >> passwd;
        cout << "\n";

        /* Set the password. */
        Extractor.setPassword(QString::fromStdString(passwd));

        /* Again start the extractor. */
        Extractor.start();
    });

    QObject::connect(&Extractor, &DiskExtractor::started, [&]() {
        qInfo() << "[+] Starting Extractor... ";
    });
    QObject::connect(&Extractor, &DiskExtractor::finished, [&]() {
        qInfo() << "[+] Extracted File(s) Successfully!";
	app.quit();
        return;
    });
    QObject::connect(&Extractor, &DiskExtractor::error, [&](short code) {
        if(code == QArchive::ArchivePasswordNeeded ||
                code == QArchive::ArchivePasswordIncorrect) {
            return;
        }
        qInfo() << "[-] An error has occured :: " << QArchive::errorCodeToString(code) ;
        app.quit();
        return;
    });
    QObject::connect(&Extractor, &DiskExtractor::info, [&](QJsonObject info) {
        qInfo() << "ARCHIVE CONTENTS:: " << info;
        return;
    });

    QObject::connect(&Extractor, &DiskExtractor::progress, 
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
