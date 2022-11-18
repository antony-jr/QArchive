#ifndef QARCHIVE_MEMORY_FILE_HPP_INCLUDED
#define QARCHIVE_MEMORY_FILE_HPP_INCLUDED
#include <QSharedPointer>
#include <QBuffer>
#include <QJsonObject>

#include "qarchive_global.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryFile {
  public:
    MemoryFile();
    MemoryFile(QJsonObject, const QSharedPointer<QBuffer>&);
    ~MemoryFile();

    QJsonObject fileInformation() const;
    QBuffer *buffer() const;
  private:
    QJsonObject m_FileInformation;
    QSharedPointer<QBuffer> m_Buffer;
};
}  // namespace QArchive

#endif // QARCHIVE_MEMORY_FILE_HPP_INCLUDED
