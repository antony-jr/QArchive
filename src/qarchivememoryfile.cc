#include "qarchivememoryfile.hpp"

using namespace QArchive;

MemoryFile::MemoryFile()
    : m_Buffer(nullptr) { }

MemoryFile::MemoryFile(QJsonObject info, const QSharedPointer<QBuffer> &buffer)
    : m_FileInformation(std::move(info)),
      m_Buffer(buffer) { }

MemoryFile::~MemoryFile() {
    m_Buffer.clear();
}

QJsonObject MemoryFile::fileInformation() const {
    return {m_FileInformation};
}

QBuffer *MemoryFile::buffer() const {
    return m_Buffer.data();
}
