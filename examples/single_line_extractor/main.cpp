#include <QArchive>
main(void){while((QArchive::Extractor("test.7z").start().waitForFinished().isRunning())){}}
