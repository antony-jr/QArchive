#ifndef QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
#include <QScopedPointer>
#include <QSharedPointer>
#include <QBuffer>
#include <QIODevice>
#include <QString>
#include <QObject>
#include <QThread>
#include <QJsonObject>
#include <QMetaType>

#include "qarchiveextractor.hpp"
#include "qarchive_global.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryExtractor : public Extractor {
    Q_OBJECT
  public:
    MemoryExtractor(QObject *parent = nullptr, bool singleThreaded = true);
    MemoryExtractor(QIODevice*, QObject *parent = nullptr, bool singleThreaded = true);
    MemoryExtractor(const QString&, QObject *parent = nullptr, bool singleThreaded = true);
    ~MemoryExtractor();
  Q_SIGNALS:
    void finished(MemoryExtractorOutput*);
};
}

#endif // QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
