#include "qarchivememoryfile.hpp"

using namespace QArchive;

MemoryFile::MemoryFile()
    : m_FileInformation(QJsonObject { }),
      m_Buffer(nullptr) { }

MemoryFile::MemoryFile(const QJsonObject &info, const QSharedPointer<QBuffer> &buffer)
    : m_FileInformation(info),
      m_Buffer(buffer) { }

MemoryFile::~MemoryFile() {
    m_Buffer.clear();
}

QJsonObject MemoryFile::fileInformation() const {
    return QJsonObject(m_FileInformation);
}

QBuffer *MemoryFile::buffer() const {
    return m_Buffer.data();
}
