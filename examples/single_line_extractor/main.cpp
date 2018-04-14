#include <QArchive>
main(void){while((QArchive::Extractor("Test4.zip").setPassword("Test4").start().waitForFinished().isRunning())){}}
