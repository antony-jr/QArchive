#ifndef QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
#include <QBuffer>
#include <QIODevice>
#include <QJsonObject>
#include <QMetaType>
#include <QObject>
#include <QString>
#include <QThread>

#include "qarchive_global.hpp"
#include "qarchiveextractor.hpp"

namespace QArchive {
class QARCHIVE_EXPORT MemoryExtractor : public Extractor {
    Q_OBJECT
public:
    explicit MemoryExtractor(QObject* parent = nullptr, bool singleThreaded = true);
    explicit MemoryExtractor(QIODevice*, QObject* parent = nullptr, bool singleThreaded = true);
    explicit MemoryExtractor(const QString&, QObject* parent = nullptr, bool singleThreaded = true);
Q_SIGNALS:
    void finished(MemoryExtractorOutput*);
};
} // namespace QArchive

#endif // QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
