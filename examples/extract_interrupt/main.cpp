#include <QCoreApplication>
#include <QDebug>
#include <QArchive>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    /*
     * 1.Construct
    */
    QArchive::Extractor e("test.7z");


    /*
     * 2.Connect Callbacks
    */

    // emitted when all extraction is finished
    QObject::connect(&e, &QArchive::Extractor::finished, [&]() {
        qDebug() << "Finished all extraction!";
	app.quit();
    });

    QObject::connect(&e, &QArchive::Extractor::extracting, [&](QString file) {
        qDebug() << "Extracting:: " << file;
    });

    // emitted when a file is extracted
    QObject::connect(&e, &QArchive::Extractor::extracted, [&](QString file) {
        qDebug() << "Extracted:: " << file;
    });

    QObject::connect(&e, &QArchive::Extractor::stopped,
    [&](){
    	qDebug() << "Stopped Successfully!";
	app.quit();
    });

    // emitted when something goes wrong
    QObject::connect(&e, &QArchive::Extractor::error, [&](short code, QString file) {
        switch(code) {
        case QArchive::ARCHIVE_READ_ERROR:
            qDebug() << "unable to find archive :: " << file;
            app.quit();
            break;
        case QArchive::ARCHIVE_QUALITY_ERROR:
            qDebug() << "bad archive! :: " << file;
            app.quit();
            break;
        case QArchive::ARCHIVE_UNCAUGHT_ERROR:
            qDebug() << "fatal error. :: " << file;
            app.quit();
            break;
        default:
            qDebug() << "unknown error. :: " << file;
	    app.quit();
            break;
        }
    });

    /*
     * 3.Start extraction!
    */
    e.start();

    QTimer::singleShot(1000 , &app , 
    [&](){ 
	qDebug() << "Stopping extraction";
    	e.stop();
    });
    qDebug() << "Its Non-Blocking!";

    return app.exec();
}
