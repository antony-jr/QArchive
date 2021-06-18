#include "qarchivememoryextractoroutput.hpp"

using namespace QArchive;

MemoryExtractorOutput::MemoryExtractorOutput(QObject *parent)
    : QObject(parent) { }

MemoryExtractorOutput::MemoryExtractorOutput(QVector<MemoryFile> *files, QObject *parent)
    : MemoryExtractorOutput(parent) {
    m_Files.reset(files);
}

MemoryExtractorOutput::~MemoryExtractorOutput() {
    m_Files.reset(nullptr);
}

QVector<MemoryFile> &MemoryExtractorOutput::getFiles() const {
    return *(m_Files.data());
}
