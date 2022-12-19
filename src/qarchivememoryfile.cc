#include "qarchivememoryfile.hpp"

using namespace QArchive;

MemoryFile::MemoryFile(QJsonObject info, std::shared_ptr<QBuffer> buffer)
    : m_FileInformation(std::move(info)),
      m_Buffer(std::move(buffer)) { }

QJsonObject MemoryFile::fileInformation() const {
    return m_FileInformation;
}

// We can't break api that easily, maybe no one is using
// this but still it's not wise to break api without chaning
// the major version which I think is a overkill so
// let's keep this for a while then remove this on the next
// major version favouring the std version.
Q_DECL_DEPRECATED QBuffer *MemoryFile::buffer() const {
    return m_Buffer.get();
}

std::shared_ptr<QBuffer> MemoryFile::sharedBuffer() const {
    return m_Buffer;
}
