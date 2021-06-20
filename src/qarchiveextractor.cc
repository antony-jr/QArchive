#include <QMetaMethod>
#include <QMetaObject>

#include "qarchiveextractor.hpp"
#include "qarchiveextractor_p.hpp"

using namespace QArchive;

static QMetaMethod getMethod(QScopedPointer<ExtractorPrivate> &object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

Extractor::Extractor(bool memoryMode, QObject *parent, bool singleThreaded)
    : QObject(parent) {
    m_Extractor.reset(new ExtractorPrivate(memoryMode));

    if(!singleThreaded) {
        m_Thread.reset(new QThread);
        m_Thread->start();
        m_Extractor->moveToThread(m_Thread.data());
    }
    connect(m_Extractor.data(), &ExtractorPrivate::started,
            this, &Extractor::started, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::canceled,
            this, &Extractor::canceled, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::paused,
            this, &Extractor::paused, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::resumed,
            this, &Extractor::resumed, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::diskFinished,
            this, &Extractor::diskFinished, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::memoryFinished,
            this, &Extractor::memoryFinished, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::error,
            this, &Extractor::error, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::progress,
            this, &Extractor::progress, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::getInfoRequirePassword,
            this, &Extractor::getInfoRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::extractionRequirePassword,
            this, &Extractor::extractionRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.data(), &ExtractorPrivate::info,
            this, &Extractor::info, Qt::DirectConnection);
}

Extractor::~Extractor() {
    if(!m_Thread.isNull()) {
        m_Thread->quit();
        m_Thread->wait();
    }
    return;
}

void Extractor::setArchive(QIODevice *archive) {
    getMethod(m_Extractor,
              "setArchive(QIODevice*)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QIODevice*, archive));
    return;
}

void Extractor::setArchive(const QString &archivePath) {
    getMethod(m_Extractor,
              "setArchive(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, archivePath));
    return;
}

void Extractor::setArchive(const QString &archivePath, const QString &outputDirectory) {
    setArchive(archivePath);
    setOutputDirectory(outputDirectory);
}

void Extractor::setBlockSize(int n) {
    getMethod(m_Extractor,
              "setBlockSize(int)").invoke(m_Extractor.data(),
                                          Qt::QueuedConnection,
                                          Q_ARG(int, n));
    return;
}

void Extractor::setCalculateProgress(bool choice) {
    getMethod(m_Extractor,
              "setCalculateProgress(bool)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(bool, choice));
    return;
}

void Extractor::setOutputDirectory(const QString &dir) {
    getMethod(m_Extractor,
              "setOutputDirectory(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, dir));
    return;
}

void Extractor::setPassword(const QString &passwd) {
    getMethod(m_Extractor,
              "setPassword(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, passwd));
    return;
}

void Extractor::addFilter(const QString &filter) {
    getMethod(m_Extractor,
              "addFilter(const QString&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, filter));
    return;
}

void Extractor::addFilter(const QStringList &filters) {
    getMethod(m_Extractor,
              "addFilter(const QStringList&)").invoke(m_Extractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QStringList, filters));
    return;
}

void Extractor::clear() {
    getMethod(m_Extractor, "clear()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void Extractor::getInfo() {
    getMethod(m_Extractor, "getInfo()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void Extractor::start() {
    getMethod(m_Extractor, "start()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void Extractor::cancel() {
    getMethod(m_Extractor, "cancel()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void Extractor::pause() {
    getMethod(m_Extractor, "pause()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}

void Extractor::resume() {
    getMethod(m_Extractor, "resume()").invoke(m_Extractor.data(), Qt::QueuedConnection);
    return;
}
