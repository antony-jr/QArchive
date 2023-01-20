#include <QMetaMethod>
#include <QMetaObject>

#include "qarchiveextractor.hpp"
#include "qarchiveextractor_p.hpp"

using namespace QArchive;

static QMetaMethod getMethod(const ExtractorPrivate &object, const char *function) {
    auto metaObject = object.metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

Extractor::Extractor(bool memoryMode, QObject *parent, bool singleThreaded)
    : QObject(parent) {
#ifdef __cpp_lib_make_unique
    m_Extractor = std::make_unique<ExtractorPrivate>(memoryMode);
#else
    m_Extractor.reset(new ExtractorPrivate(memoryMode));
#endif

    if(!singleThreaded) {
#ifdef __cpp_lib_make_unique
        m_Thread = std::make_unique<QThread>();
#else
        m_Thread.reset(new QThread);
#endif
        m_Thread->start();
        m_Extractor->moveToThread(m_Thread.get());
    }
    connect(m_Extractor.get(), &ExtractorPrivate::started,
            this, &Extractor::started, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::canceled,
            this, &Extractor::canceled, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::paused,
            this, &Extractor::paused, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::resumed,
            this, &Extractor::resumed, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::diskFinished,
            this, &Extractor::diskFinished, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::memoryFinished,
            this, &Extractor::memoryFinished, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::error,
            this, &Extractor::error, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::progress,
            this, &Extractor::progress, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::getInfoRequirePassword,
            this, &Extractor::getInfoRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::extractionRequirePassword,
            this, &Extractor::extractionRequirePassword, Qt::DirectConnection);
    connect(m_Extractor.get(), &ExtractorPrivate::info,
            this, &Extractor::info, Qt::DirectConnection);
}

Extractor::~Extractor() {
    if(m_Thread) {
        m_Thread->quit();
        m_Thread->wait();
    }
}

void Extractor::setArchive(QIODevice *archive) {
    getMethod(*m_Extractor,
              "setArchive(QIODevice*)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QIODevice*, archive));
}

void Extractor::setArchive(const QString &archivePath) {
    getMethod(*m_Extractor,
              "setArchive(const QString&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, archivePath));
}

void Extractor::setArchive(const QString &archivePath, const QString &outputDirectory) {
    setArchive(archivePath);
    setOutputDirectory(outputDirectory);
}

void Extractor::setBlockSize(int n) {
    getMethod(*m_Extractor,
              "setBlockSize(int)").invoke(m_Extractor.get(),
                                          Qt::QueuedConnection,
                                          Q_ARG(int, n));
}

void Extractor::setCalculateProgress(bool choice) {
    getMethod(*m_Extractor,
              "setCalculateProgress(bool)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(bool, choice));
}

void Extractor::setOutputDirectory(const QString &dir) {
    getMethod(*m_Extractor,
              "setOutputDirectory(const QString&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, dir));
}

void Extractor::setPassword(const QString &passwd) {
    getMethod(*m_Extractor,
              "setPassword(const QString&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, passwd));
}

void Extractor::addFilter(const QString &filter) {
    getMethod(*m_Extractor,
              "addFilter(const QString&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, filter));
}

void Extractor::addFilter(const QStringList &filters) {
    getMethod(*m_Extractor,
              "addFilter(const QStringList&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QStringList, filters));
}

void Extractor::addIncludePattern(const QString &pattern) {
    getMethod(*m_Extractor,
              "addIncludePattern(const QString&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, pattern));
}

void Extractor::addIncludePattern(const QStringList &patterns) {
    getMethod(*m_Extractor,
              "addIncludePattern(const QStringList&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QStringList, patterns));
}


void Extractor::addExcludePattern(const QString & pattern) {
    getMethod(*m_Extractor,
              "addExcludePattern(const QString&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, pattern));
}

void Extractor::addExcludePattern(const QStringList & patterns) {
    getMethod(*m_Extractor,
              "addExcludePattern(const QStringList&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QStringList, patterns));
}

void Extractor::setBasePath(const QString &path) {
    getMethod(*m_Extractor,
              "setBasePath(const QString&)").invoke(m_Extractor.get(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, path));
}

void Extractor::setRawMode(bool enabled) {
    getMethod(*m_Extractor,
        "setRawMode(bool)").invoke(m_Extractor.get(),
            Qt::QueuedConnection,
            Q_ARG(bool, enabled));
}

void Extractor::setRawOutputFilename(const QString& path) {
    getMethod(*m_Extractor,
        "setRawOutputFilename(const QString&)").invoke(m_Extractor.get(),
            Qt::QueuedConnection,
            Q_ARG(QString, path));
}

void Extractor::clear() {
    getMethod(*m_Extractor, "clear()").invoke(m_Extractor.get(), Qt::QueuedConnection);
}

void Extractor::getInfo() {
    getMethod(*m_Extractor, "getInfo()").invoke(m_Extractor.get(), Qt::QueuedConnection);
}

void Extractor::start() {
    getMethod(*m_Extractor, "start()").invoke(m_Extractor.get(), Qt::QueuedConnection);
}

void Extractor::cancel() {
    getMethod(*m_Extractor, "cancel()").invoke(m_Extractor.get(), Qt::QueuedConnection);
}

void Extractor::pause() {
    getMethod(*m_Extractor, "pause()").invoke(m_Extractor.get(), Qt::QueuedConnection);
}

void Extractor::resume() {
    getMethod(*m_Extractor, "resume()").invoke(m_Extractor.get(), Qt::QueuedConnection);
}
