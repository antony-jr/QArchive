#include <QMetaMethod>
#include <QMetaObject>

#include "qarchivediskextractor.hpp"

using namespace QArchive;

DiskExtractor::DiskExtractor(QObject *parent, bool singleThreaded)
    : Extractor(false, parent, singleThreaded) {
    connect(this, &DiskExtractor::diskFinished,
            this, &DiskExtractor::finished, Qt::DirectConnection);
}

DiskExtractor::DiskExtractor(QIODevice *archive, QObject *parent, bool singleThreaded)
    : DiskExtractor(parent, singleThreaded) {
    setArchive(archive);
}

DiskExtractor::DiskExtractor(const QString &archivePath, QObject *parent, bool singleThreaded)
    : DiskExtractor(parent, singleThreaded) {
    setArchive(archivePath);
}

DiskExtractor::DiskExtractor(const QString &archivePath, const QString &outputDirectory, QObject *parent, bool singleThreaded)
    : DiskExtractor(parent, singleThreaded) {
    setArchive(archivePath);
    setOutputDirectory(outputDirectory);
}

DiskExtractor::~DiskExtractor() { }
