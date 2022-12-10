#ifndef QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
#include <QIODevice>
#include <QString>
#include <QObject>
#include <QThread>
#include <QJsonObject>

#include "qarchiveextractor.hpp"
#include "qarchive_global.hpp"

namespace QArchive {
class QARCHIVE_EXPORT DiskExtractor : public Extractor {
    Q_OBJECT
  public:
    DiskExtractor(QObject *parent = nullptr, bool singleThreaded = true);
    DiskExtractor(QIODevice*, QObject *parent = nullptr, bool singleThreaded = true);
    DiskExtractor(const QString&, QObject *parent = nullptr, bool singleThreaded = true);
    DiskExtractor(const QString&, const QString&, QObject *parent = nullptr, bool singleThreaded = true);
  Q_SIGNALS:
    void finished();
};
}  // namespace QArchive
#endif // QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
