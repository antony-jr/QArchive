#include <QArchive>
using namespace QArchive;
int main(void)
{
    Compressor("test.7z" , QStringList() << "TestDir/README.md")
     .setFunc(QArchive::COMPRESSING , [&](QString file){
        qDebug() << "Compressing:: " << file;
     })
     .setFunc([&](short code, QString file) {
        qDebug() << "error code:: " << code << " :: " << file;
     })
     .start()
     .waitForFinished();
    return 0;
}
