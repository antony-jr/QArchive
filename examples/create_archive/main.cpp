#include <QArchive>
using namespace QArchive;
int main(void)
{
    Compressor("test.zip" , "THISDOESNOTEXISTS")
     .setFunc(QArchive::COMPRESSING , [&](QString file){
        qDebug() << "Compressing:: " << file;
     })
     .setFunc([&](short code, QString file) {
        qDebug() << "error code:: " << code << " :: " << file;
     })
     .start()
     .waitForFinished()
     .setArchive("test2.zip" , "TestDir")
     .start()
     .waitForFinished();
    return 0;
}
