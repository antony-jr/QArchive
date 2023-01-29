#ifndef QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
#include <QIODevice>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QThread>

#include "qarchive_global.hpp"
#include "qarchiveextractor.hpp"

namespace QArchive {
class QARCHIVE_EXPORT DiskExtractor : public Extractor {
  Q_OBJECT
 public:
  explicit DiskExtractor(QObject* parent = nullptr, bool singleThreaded = true);
  explicit DiskExtractor(QIODevice*, QObject* parent = nullptr,
                         bool singleThreaded = true);
  explicit DiskExtractor(const QString&, QObject* parent = nullptr,
                         bool singleThreaded = true);
  DiskExtractor(const QString&, const QString&, QObject* parent = nullptr,
                bool singleThreaded = true);
 Q_SIGNALS:
  void finished();
};
}  // namespace QArchive
#endif  // QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
