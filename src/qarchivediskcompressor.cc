#include "qarchivediskcompressor.hpp"

using namespace QArchive;

DiskCompressor::DiskCompressor(QObject *parent, bool singleThreaded)
    : Compressor(false, parent, singleThreaded) {
    connect(this, &DiskCompressor::diskFinished,
            this, &DiskCompressor::finished, Qt::DirectConnection);
}

DiskCompressor::DiskCompressor(const QString &fileName, QObject *parent, bool singleThreaded)
    : DiskCompressor(parent, singleThreaded) {
    setFileName(fileName);
}

DiskCompressor::DiskCompressor(const QString &fileName, short format, QObject *parent, bool singleThreaded)
    : DiskCompressor(parent, singleThreaded) {
    setFileName(fileName);
    setArchiveFormat(format);
}

DiskCompressor::~DiskCompressor() = default;
