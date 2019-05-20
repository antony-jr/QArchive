#include <QMetaMethod>
#include <QMetaObject>

#include <qarchivediskextractor.hpp>
#include <qarchivediskextractor_p.hpp>

using namespace QArchive;

static QMetaMethod getMethod(QScopedPointer<DiskExtractorPrivate> &object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

DiskExtractor::DiskExtractor(QObject *parent, bool singleThreaded)
    : QObject(parent) {
    m_Extractor.reset(new DiskExtractorPrivate);

    if(!singleThreaded) {
        m_Thread.reset(new QThread);
        m_Thread->start();
        m_Extractor->moveToThread(m_Thread.data());
    }
    connect(m_Extractor.data(), &DiskExtractorPrivate::started,
            this, &DiskExtractor::started, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::canceled,
            this, &DiskExtractor::canceled, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::paused,
            this, &DiskExtractor::paused, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::resumed,
            this, &DiskExtractor::resumed, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::finished,
            this, &DiskExtractor::finished, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::error,
            this, &DiskExtractor::error, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::progress,
            this, &DiskExtractor::progress, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::getInfoRequirePassword,
            this, &DiskExtractor::getInfoRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::extractionRequirePassword,
            this, &DiskExtractor::extractionRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.data(), &DiskExtractorPrivate::info,
            this, &DiskExtractor::info, Qt::DirectConnection);
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


DiskExtractor::~DiskExtractor() {
    if(!m_Thread.isNull()) {
        m_Thread->quit();
        m_Thread->wait();
    }
    return;
}

void DiskExtractor::setArchive(QIODevice *archive) {
    getMethod(m_Extractor,
              "setArchive(QIODevice*)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QIODevice*, archive));
    return;
}

void DiskExtractor::setArchive(const QString &archivePath) {
    getMethod(m_Extractor,
              "setArchive(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, archivePath));
    return;
}

void DiskExtractor::setArchive(const QString &archivePath, const QString &outputDirectory) {
    setArchive(archivePath);
    setOutputDirectory(outputDirectory);
}

void DiskExtractor::setBlockSize(int n) {
    getMethod(m_Extractor,
              "setBlockSize(int)").invoke(m_Extractor.data(),
                                          Qt::QueuedConnection,
                                          Q_ARG(int, n));
    return;
}

void DiskExtractor::setCalculateProgress(bool choice) {
    getMethod(m_Extractor,
              "setCalculateProgress(bool)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(bool, choice));
    return;
}

void DiskExtractor::setOutputDirectory(const QString &dir) {
    getMethod(m_Extractor,
              "setOutputDirectory(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, dir));
    return;
}

void DiskExtractor::setPassword(const QString &passwd) {
    getMethod(m_Extractor,
              "setPassword(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, passwd));
    return;
}

void DiskExtractor::addFilter(const QString &filter) {
    getMethod(m_Extractor,
              "addFilter(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, filter));
    return;
}

void DiskExtractor::addFilter(const QStringList &filters) {
    getMethod(m_Extractor,
              "addFilter(const QStringList&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QStringList, filters));
    return;
}

void DiskExtractor::clear() {
    getMethod(m_Extractor, "clear()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::getInfo() {
    getMethod(m_Extractor, "getInfo()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::start() {
    getMethod(m_Extractor, "start()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::cancel() {
    getMethod(m_Extractor, "cancel()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::pause() {
    getMethod(m_Extractor, "pause()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::resume() {
    getMethod(m_Extractor, "resume()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}
