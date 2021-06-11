#include <QMetaMethod>
#include <QMetaObject>

#include "qarchivememoryextractor.hpp"
#include "qarchiveextractor_p.hpp"

using namespace QArchive;

static QMetaMethod getMethod(QScopedPointer<ExtractorPrivate> &object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

MemoryExtractor::MemoryExtractor(QObject *parent, bool singleThreaded)
    : QObject(parent) {
    m_Extractor.reset(new ExtractorPrivate(/*Memory Mode=*/true));

    if(!singleThreaded) {
        m_Thread.reset(new QThread);
        m_Thread->start();
        m_Extractor->moveToThread(m_Thread.data());
    }
    
    connect(m_Extractor.data(), &ExtractorPrivate::started,
            this, &MemoryExtractor::started, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::canceled,
            this, &MemoryExtractor::canceled, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::paused,
            this, &MemoryExtractor::paused, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::resumed,
            this, &MemoryExtractor::resumed, Qt::DirectConnection);
    connect(m_Extractor.data(), 
	    SIGNAL(finished(QVector<QPair<QJsonObject, QSharedPointer<QBuffer>>>*)),
            this, 
	    SIGNAL(finished(QVector<QPair<QJsonObject, QSharedPointer<QBuffer>>>*)),
	    Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::error,
            this, &MemoryExtractor::error, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::progress,
            this, &MemoryExtractor::progress, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::getInfoRequirePassword,
            this, &MemoryExtractor::getInfoRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::extractionRequirePassword,
            this, &MemoryExtractor::extractionRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::info,
            this, &MemoryExtractor::info, Qt::DirectConnection);
}

MemoryExtractor::MemoryExtractor(QIODevice *archive, QObject *parent, bool singleThreaded)
    : MemoryExtractor(parent, singleThreaded) {
    setArchive(archive);
}

MemoryExtractor::MemoryExtractor(const QString &archivePath, QObject *parent, bool singleThreaded)
    : MemoryExtractor(parent, singleThreaded) {
    setArchive(archivePath);
}

MemoryExtractor::~MemoryExtractor() {
    if(!m_Thread.isNull()) {
        m_Thread->quit();
        m_Thread->wait();
    }
    return;
}

void MemoryExtractor::setArchive(QIODevice *archive) {
    getMethod(m_Extractor,
              "setArchive(QIODevice*)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QIODevice*, archive));
    return;
}

void MemoryExtractor::setArchive(const QString &archivePath) {
    getMethod(m_Extractor,
              "setArchive(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, archivePath));
    return;
}

void MemoryExtractor::setBlockSize(int n) {
    getMethod(m_Extractor,
              "setBlockSize(int)").invoke(m_Extractor.data(),
                                          Qt::QueuedConnection,
                                          Q_ARG(int, n));
    return;
}

void MemoryExtractor::setCalculateProgress(bool choice) {
    getMethod(m_Extractor,
              "setCalculateProgress(bool)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(bool, choice));
    return;
}

void MemoryExtractor::setPassword(const QString &passwd) {
    getMethod(m_Extractor,
              "setPassword(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, passwd));
    return;
}

void MemoryExtractor::addFilter(const QString &filter) {
    getMethod(m_Extractor,
              "addFilter(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, filter));
    return;
}

void MemoryExtractor::addFilter(const QStringList &filters) {
    getMethod(m_Extractor,
              "addFilter(const QStringList&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QStringList, filters));
    return;
}

void MemoryExtractor::clear() {
    getMethod(m_Extractor, "clear()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void MemoryExtractor::getInfo() {
    getMethod(m_Extractor, "getInfo()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void MemoryExtractor::start() {
    getMethod(m_Extractor, "start()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void MemoryExtractor::cancel() {
    getMethod(m_Extractor, "cancel()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void MemoryExtractor::pause() {
    getMethod(m_Extractor, "pause()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void MemoryExtractor::resume() {
    getMethod(m_Extractor, "resume()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}
