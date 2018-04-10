#include <QCoreApplication>
#include <QDebug>
#include <QArchive>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    QArchive::Compressor c;
    c.setArchive("test.7z")
     .addFiles(QStringList() << "TestDir/Ed_Sheeran_-_Castle_On_The_Hill_Official_Lyric_Video-7Qp5vcuMIlk.webm"
                             << "TestDir/README.md"
                             << "TestDir/Ed_Sheeran_-_Thinking_Out_Loud_Official_Video-lp-EO5I60KA.mkv"
     )
     .setFunc(QArchive::COMPRESSING , [&](QString file){
        qDebug() << "Compressing:: " << file;
     })
     .setFunc(QArchive::FINISHED , [&]() {
        qDebug() << "Finished all jobs";
        app.quit();
     })
     .setFunc([&](short code, QString file) {
        qDebug() << "error code:: " << code << " :: " << file;
        app.quit();
     })
     .start();
    return app.exec();
}
