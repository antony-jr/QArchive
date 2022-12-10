#include "qarchivememoryextractoroutput.hpp"

using namespace QArchive;

MemoryExtractorOutput::MemoryExtractorOutput(std::unique_ptr<QVector<MemoryFile>> files, QObject *parent)
    : MemoryExtractorOutput(parent) {
    m_Files = std::move(files);
}

const QVector<MemoryFile> &MemoryExtractorOutput::getFiles() const {
    return *m_Files;
}
