#ifndef QARCHIVE_MEMORY_FILE_HPP_INCLUDED
#define QARCHIVE_MEMORY_FILE_HPP_INCLUDED
#include <QBuffer>
#include <QJsonObject>

#include "qarchive_global.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryFile {
  public:
    MemoryFile();
    MemoryFile(QJsonObject, std::shared_ptr<QBuffer>);
    ~MemoryFile();

    MemoryFile(const MemoryFile&) = default;
    MemoryFile& operator=(const MemoryFile&) = default;

    [[gnu::warn_unused_result]] QJsonObject fileInformation() const;
    [[gnu::warn_unused_result]] QBuffer *buffer() const;
  private:
    QJsonObject m_FileInformation;
    std::shared_ptr<QBuffer> m_Buffer;
};
}  // namespace QArchive

#endif // QARCHIVE_MEMORY_FILE_HPP_INCLUDED
