#include <QArchive>
using namespace QArchive;
int main(void)
{
    Compressor("test.7z" , "TestDir")
     .setPassword("test123") // May not work! 
     .setCompressionLevel(9)
     .setBlocksize(4096)
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
