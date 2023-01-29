#ifndef QARCHIVE_MEMORY_COMPRESSOR_HPP_INCLUDED
#define QARCHIVE_MEMORY_COMPRESSOR_HPP_INCLUDED
#include <QBuffer>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QThread>

#include "qarchive_global.hpp"
#include "qarchivecompressor.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryCompressor : public Compressor {
  Q_OBJECT
 public:
  explicit MemoryCompressor(QObject* parent = nullptr,
                            bool singleThreaded = true);
  explicit MemoryCompressor(short, QObject* parent = nullptr,
                            bool singleThreaded = true);
 Q_SIGNALS:
  void finished(QBuffer*);
};
}  // namespace QArchive
#endif  // QARCHIVE_MEMORY_COMPRESSOR_HPP_INCLUDED
