#include <QArchive>

int main(int ac, char **av)
{
    if(ac == 1) {
        qDebug() << "Usage: " << av[0] << " [ARCHIVE PATH]";
        return 0;
    }
    QString ArchivePath = QString(av[1]);
    qDebug() << "Starting the magic... ";
    // Read.
    qDebug() << "Files:: "
             << QArchive::Reader(ArchivePath).start().waitForFinished().getFilesList();
    // Extract.
    qDebug() << "Starting Extraction... ";
    QArchive::Extractor(ArchivePath).start().waitForFinished();
    qDebug() << "Extraction was Successfull!";
    return 0;
}
