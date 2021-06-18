#include "qarchivememoryextractor.hpp"

using namespace QArchive;

MemoryExtractor::MemoryExtractor(QObject *parent, bool singleThreaded)
    : Extractor(/*Memory Mode=*/true, parent, singleThreaded) {

    connect(this, &MemoryExtractor::memoryFinished,
            this, &MemoryExtractor::finished, Qt::DirectConnection);
}

MemoryExtractor::MemoryExtractor(QIODevice *archive, QObject *parent, bool singleThreaded)
    : MemoryExtractor(parent, singleThreaded) {
    setArchive(archive);
}

MemoryExtractor::MemoryExtractor(const QString &archivePath, QObject *parent, bool singleThreaded)
    : MemoryExtractor(parent, singleThreaded) {
    setArchive(archivePath);
}

MemoryExtractor::~MemoryExtractor() { }
