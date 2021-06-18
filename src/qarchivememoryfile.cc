#include "qarchivememoryfile.hpp"

using namespace QArchive;

MutableMemoryFile::MutableMemoryFile() { }
MutableMemoryFile::~MutableMemoryFile() {
    m_Buffer.clear();
}


void MutableMemoryFile::setFileInformation(const QJsonObject &info) {

}

void MutableMemoryFile::setBuffer(QBuffer *buffer) {
    m_Buffer.reset(buffer);
}

QJsonObject MutableMemoryFile::getFileInformation() {
    return m_FileInformation;
}

QSharedPointer<QBuffer> MutableMemoryFile::getBuffer() {
    return m_Buffer;
}

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
    return m_FileInformation;
}

QBuffer *MemoryFile::buffer() const {
    return m_Buffer.data();
}
