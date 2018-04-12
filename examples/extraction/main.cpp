#include <QCoreApplication>
#include <QDebug>
#include <QArchive>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QArchive::Extractor e;

    bool once = true;

    e.setArchive("test.7z")
    .setFunc(QArchive::EXTRACTING, [&](QString file) {
        qDebug() << "Extracting:: " << file;
    })
    .setFunc(QArchive::EXTRACTED, [&](QString file) {
        qDebug() << "Extracted:: " << file;
        if(once){
        e.pause();
        once &= false;
        }
    })
    .setFunc([&](short code, QString file) {
        switch(code) {
        case QArchive::ARCHIVE_READ_ERROR:
            qDebug() << "unable to find archive :: " << file;
            e.cancel();
            app.quit();
            break;
        case QArchive::ARCHIVE_QUALITY_ERROR:
            qDebug() << "bad archive! :: " << file;
            e.cancel();
            app.quit();
            break;
        case QArchive::ARCHIVE_UNCAUGHT_ERROR:
            qDebug() << "fatal error. :: " << file;
            e.cancel();
            app.quit();
            break;
        default:
            qDebug() << "unknown error. :: " << file;
            e.cancel();
            app.quit();
            break;
        }
    })
    .setFunc(QArchive::PAUSED , [&]() {
        qDebug() << "Paused";
        qDebug() << "Resuming in 5 Seconds!";
        QTimer::singleShot(5000 , &e , SLOT(resume()));
    })
    .setFunc(QArchive::FINISHED, [&]() {
        qDebug() << "Finished all extraction!";
        app.quit();
    })
    .start();
    qDebug() << "Its Non Blocking!";
    return app.exec();
}
