#ifndef QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
#include <QObject>
#include <QVector>
#include <QScopedPointer>

#include "qarchivememoryfile.hpp"
#include "qarchive_global.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryExtractorOutput : public QObject {
    Q_OBJECT
  public:
    MemoryExtractorOutput(QObject *parent = nullptr);
    MemoryExtractorOutput(QVector<MemoryFile>*, QObject *parent = nullptr);
    ~MemoryExtractorOutput();

    QVector<MemoryFile> &getFiles() const;

  private:
    QScopedPointer<QVector<MemoryFile>> m_Files;
};
}
#endif // QARCHIVE_MEMORY_EXTRACTOR_OUTPUT_HPP_INCLUDED
