#ifndef QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
#include <QObject>
#include <QVector>

#include <memory>

#include "qarchive_global.hpp"
#include "qarchivememoryfile.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryExtractorOutput : public QObject {
  using QObject::QObject;
  Q_OBJECT
 public:
  explicit MemoryExtractorOutput(std::unique_ptr<QVector<MemoryFile>>,
                                 QObject* parent = nullptr);

  [[gnu::warn_unused_result]] const QVector<MemoryFile>& getFiles() const;

 private:
  std::unique_ptr<QVector<MemoryFile>> m_Files;
};
}  // namespace QArchive
#endif  // QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
