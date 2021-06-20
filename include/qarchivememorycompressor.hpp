#ifndef QARCHIVE_MEMORY_COMPRESSOR_HPP_INCLUDED
#define QARCHIVE_MEMORY_COMPRESSOR_HPP_INCLUDED
#include <QObject>
#include <QBuffer>
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QThread>

#include "qarchive_global.hpp"
#include "qarchivecompressor.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryCompressor : public Compressor {
    Q_OBJECT
  public:
    MemoryCompressor(QObject *parent = nullptr, bool singleThreaded = true);
    MemoryCompressor(short, QObject *parent = nullptr, bool singleThreaded = true);
    ~MemoryCompressor();
  Q_SIGNALS:
    void finished(QBuffer*);
};
}
#endif // QARCHIVE_MEMORY_COMPRESSOR_HPP_INCLUDED
