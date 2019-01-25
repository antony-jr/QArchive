#include <iostream>
#include <string>
#include <QCoreApplication>
#include <QDebug>
#include <QArchive>

int main(int ac, char **av)
{
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
    DiskExtractor Extractor(ArchivePath , DestinationPath);
    /* Connect Signals with Slots (in this case lambda functions). */
    QObject::connect(&Extractor , &DiskExtractor::extractionRequirePassword , [&](int tries){
	string passwd;
	cout << "[*] Please Enter Archive Password(Tries = " << tries << "):: ";
	cin >> passwd;
	cout << "\n";

	/* Set the password. */
	Extractor.setPassword(QString::fromStdString(passwd));

	/* Again start the extractor. */
	Extractor.start();	
    });

    QObject::connect(&Extractor , &DiskExtractor::started , [&](){
        qInfo() << "[+] Starting Extractor... ";
    });
    QObject::connect(&Extractor , &DiskExtractor::finished , [&](){
        qInfo() << "[+] Extracted File(s) Successfully!";
        app.quit();
        return;
    });
    QObject::connect(&Extractor , &DiskExtractor::error , [&](short code , QString archive){
        if(code == QArchive::ArchivePasswordNeeded || 
	   code == QArchive::ArchivePasswordIncorrect){
		(void)archive;
		return;
	}	
	qInfo() << "[-] An error has occured :: " << DiskExtractor::errorCodeToString(code) << ":: " << archive ;
        app.quit();
        return;
    });

    /* Start the Extractor. 
     * Note:
     *    You don't really have to worry about the event loop since all method calls 
     *    to the extractor object is queued and only gets executed when the event
     *    loop starts , most likely.
    */
    Extractor.start(); 
    return app.exec();
}
