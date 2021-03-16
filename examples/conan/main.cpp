#include <QCoreApplication>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QArchive/QArchive>


int main(int ac, char **av) {
        QCoreApplication app(ac, av);
        QTemporaryDir dir;
        QString data = QString::fromUtf8("Some Text to Compress.");
        QFile textFile;
        textFile.setFileName(dir.path() + QDir::separator() + "TextFile.txt");
        if(!textFile.open(QIODevice::WriteOnly)) {
                return -1;
        }

        textFile.write(data.toUtf8());
        textFile.close();

        QArchive::DiskCompressor compressor;
        QArchive::DiskExtractor extractor;

        compressor.setFileName(dir.path() + QDir::separator() + "Test.zip");
        compressor.addFiles(QString::fromUtf8("Output.txt") , dir.path() + QDir::separator() + "TextFile.txt");

        extractor.setArchive(dir.path() + QDir::separator() + "Test.zip");
        extractor.setOutputDirectory(dir.path());

        QObject::connect(&compressor, &QArchive::DiskCompressor::finished,
        [&]() {
                extractor.start();
        });

        QObject::connect(&extractor, &QArchive::DiskExtractor::finished,
        [&]() {
                /// Check if the data is correct.
                QFile file;
                file.setFileName(dir.path() + QDir::separator() + "Output.txt");
                if(!file.open(QIODevice::ReadOnly)){
                        qDebug() << "Cannot Open Output.txt!";
                        app.quit();
                }

                auto got = QString(file.readAll());
                file.close();

                if(got != data) {
                        qDebug() << "Data does not Match: " << got;
                        app.quit();
                }
                qDebug() << "Data: " << got;
                qDebug() << "Test Finished.";
                app.quit();
        });

        compressor.start();
        return app.exec();
}
