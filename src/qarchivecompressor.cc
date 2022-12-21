#include <QMetaMethod>
#include <QMetaObject>

#include "qarchivecompressor.hpp"
#include "qarchivecompressor_p.hpp"

using namespace QArchive;

static QMetaMethod getMethod(const CompressorPrivate &object, const char *function) {
    auto metaObject = object.metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

Compressor::Compressor(bool memoryMode, QObject *parent, bool singleThreaded)
    : QObject(parent) {
#ifdef __cpp_lib_make_unique
    m_Compressor = std::make_unique<CompressorPrivate>(memoryMode);
#else
    m_Compressor.reset(new CompressorPrivate(memoryMode));
#endif
    if(!singleThreaded) {
#ifdef __cpp_lib_make_unique
        m_Thread = std::make_unique<QThread>();
#else
        m_Thread.reset(new QThread);
#endif
        m_Thread->start();
        m_Compressor->moveToThread(m_Thread.get());
    }
    connect(m_Compressor.get(), &CompressorPrivate::progress,
            this, &Compressor::progress, Qt::DirectConnection);
    connect(m_Compressor.get(), &CompressorPrivate::error,
            this, &Compressor::error, Qt::DirectConnection);
    connect(m_Compressor.get(), &CompressorPrivate::started,
            this, &Compressor::started, Qt::DirectConnection);
    connect(m_Compressor.get(), &CompressorPrivate::canceled,
            this, &Compressor::canceled, Qt::DirectConnection);
    connect(m_Compressor.get(), &CompressorPrivate::paused,
            this, &Compressor::paused, Qt::DirectConnection);
    connect(m_Compressor.get(), &CompressorPrivate::resumed,
            this, &Compressor::resumed, Qt::DirectConnection);
    connect(m_Compressor.get(), &CompressorPrivate::memoryFinished,
            this, &Compressor::memoryFinished, Qt::DirectConnection);
    connect(m_Compressor.get(), &CompressorPrivate::diskFinished,
            this, &Compressor::diskFinished, Qt::DirectConnection);
}

Compressor::~Compressor() {
    if(m_Thread) {
        m_Thread->quit();
        m_Thread->wait();
    }
}

void Compressor::setFileName(const QString &fileName) {
    getMethod(*m_Compressor, "setFileName(const QString&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QString, fileName));
}

void Compressor::setArchiveFormat(short format) {
    getMethod(*m_Compressor, "setArchiveFormat(short)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(short, format));
}

void Compressor::setPassword (const QString &passwd) {
    getMethod(*m_Compressor, "setPassword(const QString&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QString, passwd));
}

void Compressor::setBlockSize(int size) {
    getMethod(*m_Compressor, "setBlockSize(int)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(int, size));
}

void Compressor::addFiles(const QString &entry, QIODevice *io) {
    getMethod(*m_Compressor, "addFiles(const QString&, QIODevice*)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QString, entry), Q_ARG(QObject*, io));
}

void Compressor::addFiles(const QStringList &entries, const QVariantList &devices) {
    getMethod(*m_Compressor, "addFiles(const QStringList&, const QVariantList&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QStringList, entries), Q_ARG(QVariantList, devices));
}

void Compressor::addFiles(const QString &file) {
    getMethod(*m_Compressor, "addFiles(const QString&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QString,file));
}

void Compressor::addFiles(const QStringList &files) {
    getMethod(*m_Compressor, "addFiles(const QStringList&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QStringList, files));
}

void Compressor::addFiles(const QString &entryName, const QString &file) {
    getMethod(*m_Compressor, "addFiles(const QString& , const QString&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QString, entryName), Q_ARG(QString, file));
}

void Compressor::addFiles(const QStringList &entryNames, const QStringList &files) {
    getMethod(*m_Compressor, "addFiles(const QStringList& , const QStringList&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QStringList,entryNames),Q_ARG(QStringList, files));
}

void Compressor::removeFiles(const QString &file) {
    getMethod(*m_Compressor, "removeFiles(const QString&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QString,file));
}

void Compressor::removeFiles(const QStringList &files) {
    getMethod(*m_Compressor, "removeFiles(const QStringList&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QStringList, files));
}

void Compressor::removeFiles(const QString &entryName, const QString &file) {
    getMethod(*m_Compressor, "removeFiles(const QString& , const QString&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QString, entryName), Q_ARG(QString, file));
}

void Compressor::removeFiles(const QStringList &entryNames, const QStringList &files) {
    getMethod(*m_Compressor, "removeFiles(const QStringList& , const QStringList&)")
    .invoke(m_Compressor.get(), Qt::QueuedConnection, Q_ARG(QStringList,entryNames),Q_ARG(QStringList, files));
}

void Compressor::clear() {
    getMethod(*m_Compressor, "clear()")
    .invoke(m_Compressor.get(), Qt::QueuedConnection);
}

void Compressor::start() {
    getMethod(*m_Compressor, "start()")
    .invoke(m_Compressor.get(), Qt::QueuedConnection);
}

void Compressor::cancel() {
    getMethod(*m_Compressor, "cancel()")
    .invoke(m_Compressor.get(), Qt::QueuedConnection);
}

void Compressor::pause() {
    getMethod(*m_Compressor, "pause()")
    .invoke(m_Compressor.get(),Qt::QueuedConnection);
}

void Compressor::resume() {
    getMethod(*m_Compressor, "resume()")
    .invoke(m_Compressor.get(), Qt::QueuedConnection);
}
