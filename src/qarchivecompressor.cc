#include <QMetaMethod>
#include <QMetaObject>

#include "qarchivecompressor.hpp"
#include "qarchivecompressor_p.hpp"

using namespace QArchive;

static QMetaMethod getMethod(QScopedPointer<CompressorPrivate> &object, const char *function) {
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

Compressor::Compressor(bool memoryMode, QObject *parent, bool singleThreaded)
    : QObject(parent) {
    m_Compressor.reset(new CompressorPrivate(memoryMode));
    if(!singleThreaded) {
        m_Thread.reset(new QThread);
        m_Thread->start();
        m_Compressor->moveToThread(m_Thread.data());
    }
    connect(m_Compressor.data(), &CompressorPrivate::progress,
            this, &Compressor::progress, Qt::DirectConnection);
    connect(m_Compressor.data(), &CompressorPrivate::error,
            this, &Compressor::error, Qt::DirectConnection);
    connect(m_Compressor.data(), &CompressorPrivate::started,
            this, &Compressor::started, Qt::DirectConnection);
    connect(m_Compressor.data(), &CompressorPrivate::canceled,
            this, &Compressor::canceled, Qt::DirectConnection);
    connect(m_Compressor.data(), &CompressorPrivate::paused,
            this, &Compressor::paused, Qt::DirectConnection);
    connect(m_Compressor.data(), &CompressorPrivate::resumed,
            this, &Compressor::resumed, Qt::DirectConnection);
    connect(m_Compressor.data(), &CompressorPrivate::memoryFinished,
            this, &Compressor::memoryFinished, Qt::DirectConnection);
    connect(m_Compressor.data(), &CompressorPrivate::diskFinished,
            this, &Compressor::diskFinished, Qt::DirectConnection);
    return;
}

Compressor::~Compressor() {
    if(!m_Thread.isNull()) {
        m_Thread->quit();
        m_Thread->wait();
    }
    return;
}

void Compressor::setFileName(const QString &fileName) {
    getMethod(m_Compressor, "setFileName(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, fileName));
    return;
}

void Compressor::setArchiveFormat(short format) {
    getMethod(m_Compressor, "setArchiveFormat(short)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(short, format));
    return;
}

void Compressor::setPassword (const QString &passwd) {
    getMethod(m_Compressor, "setPassword(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, passwd));
    return;
}

void Compressor::setBlockSize(int size) {
    getMethod(m_Compressor, "setBlockSize(int)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(int, size));
    return;
}

void Compressor::addFiles(const QString &entry, QIODevice *io) {
    getMethod(m_Compressor, "addFiles(const QString&, QIODevice*)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, entry), Q_ARG(QIODevice*, io));
}

void Compressor::addFiles(const QStringList &entries, const QVariantList &devices) {
    getMethod(m_Compressor, "addFiles(const QStringList&, const QVariantList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList, entries), Q_ARG(QVariantList, devices));
}

void Compressor::addFiles(const QString &file) {
    getMethod(m_Compressor, "addFiles(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString,file));
    return;
}

void Compressor::addFiles(const QStringList &files) {
    getMethod(m_Compressor, "addFiles(const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList, files));
    return;
}

void Compressor::addFiles(const QString &entryName, const QString &file) {
    getMethod(m_Compressor, "addFiles(const QString& , const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, entryName), Q_ARG(QString, file));
    return;
}

void Compressor::addFiles(const QStringList &entryNames, const QStringList &files) {
    getMethod(m_Compressor, "addFiles(const QStringList& , const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList,entryNames),Q_ARG(QStringList, files));
    return;
}

void Compressor::removeFiles(const QString &file) {
    getMethod(m_Compressor, "removeFiles(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString,file));
    return;
}

void Compressor::removeFiles(const QStringList &files) {
    getMethod(m_Compressor, "removeFiles(const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList, files));
    return;
}

void Compressor::removeFiles(const QString &entryName, const QString &file) {
    getMethod(m_Compressor, "removeFiles(const QString& , const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, entryName), Q_ARG(QString, file));
    return;
}

void Compressor::removeFiles(const QStringList &entryNames, const QStringList &files) {
    getMethod(m_Compressor, "removeFiles(const QStringList& , const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList,entryNames),Q_ARG(QStringList, files));
    return;
}

void Compressor::clear() {
    getMethod(m_Compressor, "clear()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}

void Compressor::start() {
    getMethod(m_Compressor, "start()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}

void Compressor::cancel() {
    getMethod(m_Compressor, "cancel()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}

void Compressor::pause() {
    getMethod(m_Compressor, "pause()")
    .invoke(m_Compressor.data(),Qt::QueuedConnection);
    return;
}

void Compressor::resume() {
    getMethod(m_Compressor, "resume()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}
