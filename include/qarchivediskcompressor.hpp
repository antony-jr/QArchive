#ifndef QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
#define QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QThread>

#include "qarchive_global.hpp"
#include "qarchivecompressor.hpp"

namespace QArchive {
class QARCHIVE_EXPORT DiskCompressor : public Compressor {
    Q_OBJECT
  public:
    DiskCompressor(QObject *parent = nullptr, bool singleThreaded = true);
    DiskCompressor(const QString&, QObject *parent = nullptr,  bool singleThreaded = true);
    DiskCompressor(const QString&, short, QObject *parent = nullptr, bool singleThreaded = true);
    ~DiskCompressor();
  Q_SIGNALS:
    void finished();
};
}
#endif // QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
