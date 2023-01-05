#ifndef QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
#define QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QStringList>
#include <QThread>

#include "qarchive_global.hpp"
#include "qarchivecompressor.hpp"

namespace QArchive {
class QARCHIVE_EXPORT DiskCompressor : public Compressor {
    Q_OBJECT
public:
    explicit DiskCompressor(QObject* parent = nullptr, bool singleThreaded = true);
    explicit DiskCompressor(const QString&, QObject* parent = nullptr, bool singleThreaded = true);
    DiskCompressor(const QString&, short, QObject* parent = nullptr, bool singleThreaded = true);
Q_SIGNALS:
    void finished();
};
} // namespace QArchive
#endif // QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
