#include "qarchivememoryextractoroutput.hpp"

using namespace QArchive;

MemoryExtractorOutput::MemoryExtractorOutput(QVector<MemoryFile> *files, QObject *parent)
    : MemoryExtractorOutput(parent) {
    m_Files = *files;
}

const QVector<MemoryFile> &MemoryExtractorOutput::getFiles() const {
    return m_Files;
}
