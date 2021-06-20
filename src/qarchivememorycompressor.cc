#include "qarchivememorycompressor.hpp"

using namespace QArchive;

MemoryCompressor::MemoryCompressor(QObject *parent, bool singleThreaded)
    : Compressor(true, parent, singleThreaded) {
    connect(this, &MemoryCompressor::memoryFinished,
            this, &MemoryCompressor::finished, Qt::DirectConnection);
    return;
}

MemoryCompressor::MemoryCompressor(short format, QObject *parent, bool singleThreaded)
    : MemoryCompressor(parent, singleThreaded) {
    setArchiveFormat(format);
    return;
}

MemoryCompressor::~MemoryCompressor() { }
