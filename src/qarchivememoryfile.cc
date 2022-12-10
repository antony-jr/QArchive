#include "qarchivememoryfile.hpp"

using namespace QArchive;

MemoryFile::MemoryFile(QJsonObject info, std::shared_ptr<QBuffer> buffer)
    : m_FileInformation(std::move(info)),
      m_Buffer(std::move(buffer)) { }

QJsonObject MemoryFile::fileInformation() const {
    return {m_FileInformation};
}

QBuffer *MemoryFile::buffer() const {
    return m_Buffer.get();
}
