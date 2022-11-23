#ifndef QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
#include <QObject>
#include <QVector>

#include "qarchivememoryfile.hpp"
#include "qarchive_global.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryExtractorOutput : public QObject {
  using QObject::QObject;
    Q_OBJECT
  public:
    MemoryExtractorOutput(QVector<MemoryFile>*, QObject *parent = nullptr);

    const QVector<MemoryFile> &getFiles() const;

  private:
    QVector<MemoryFile> m_Files;
};
}  // namespace QArchive
#endif // QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
