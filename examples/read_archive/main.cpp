#include <QCoreApplication>
#include <QDebug>
#include <QArchive>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QArchive::Reader("test.7z")
    .setFunc([&](QJsonObject files){
        qDebug() << files;
    })
    .start().waitForFinished();
    return app.exec();
}
