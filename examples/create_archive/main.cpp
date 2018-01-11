#include <QCoreApplication>
#include <QDebug>
#include "QArchive.hpp"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    /*
     * 1.Construct
    */
    QArchive::Compressor e("test.7z", "TestDir");
    //			Archive		|--> Can also
    //			can be		     simply add
    //			detected	     directories.
    //			with extension.

    /*
     * 2.Connect Callbacks
    */

    // emitted when all extraction is finished
    QObject::connect(&e, &QArchive::Compressor::finished, [&]() {
        qDebug() << "Finished all jobs";
        app.quit();
    });
    QObject::connect(&e, &QArchive::Compressor::error, [&](short code, QString file) {
        qDebug() << "error code:: " << code << " :: " << file;
        app.quit();
    });
    /*
     * 3.Start extraction!
    */
    e.start();

    qDebug() << "Its Non-Blocking!";

    return app.exec();
}
