#include <QMetaMethod>
#include <QMetaObject>

#include <qarchivediskcompressor.hpp>
#include <qarchivediskcompressor_p.hpp>

using namespace QArchive;

static QMetaMethod getMethod(QScopedPointer<DiskCompressorPrivate> &object, const char *function)
{
    auto metaObject = object->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(function)));
}

DiskCompressor::DiskCompressor(QObject *parent, bool singleThreaded)
    : QObject(parent)
{
    m_Compressor.reset(new DiskCompressorPrivate);
    if(!singleThreaded) {
        m_Thread.reset(new QThread);
        m_Thread->start();
        m_Compressor->moveToThread(m_Thread.data());
    }
    connect(m_Compressor.data(), &DiskCompressorPrivate::progress,
            this, &DiskCompressor::progress, Qt::DirectConnection);
    connect(m_Compressor.data(), &DiskCompressorPrivate::error,
            this, &DiskCompressor::error, Qt::DirectConnection);
    connect(m_Compressor.data(), &DiskCompressorPrivate::started,
            this, &DiskCompressor::started, Qt::DirectConnection);
    connect(m_Compressor.data(), &DiskCompressorPrivate::canceled,
            this, &DiskCompressor::canceled, Qt::DirectConnection);
    connect(m_Compressor.data(), &DiskCompressorPrivate::paused,
            this, &DiskCompressor::paused, Qt::DirectConnection);
    connect(m_Compressor.data(), &DiskCompressorPrivate::resumed,
            this, &DiskCompressor::resumed, Qt::DirectConnection);
    connect(m_Compressor.data(), &DiskCompressorPrivate::finished,
            this, &DiskCompressor::finished, Qt::DirectConnection);
    return;
}

DiskCompressor::DiskCompressor(const QString &fileName, QObject *parent, bool singleThreaded)
    : DiskCompressor(parent, singleThreaded)
{
    setFileName(fileName);
    return;
}

DiskCompressor::DiskCompressor(const QString &fileName, short format, QObject *parent, bool singleThreaded)
    : DiskCompressor(parent, singleThreaded)
{
    setFileName(fileName);
    setArchiveFormat(format);
    return;
}

DiskCompressor::~DiskCompressor()
{
}

void DiskCompressor::setFileName(const QString &fileName)
{
    getMethod(m_Compressor, "setFileName(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, fileName));
    return;
}

void DiskCompressor::setArchiveFormat(short format)
{
    getMethod(m_Compressor, "setArchiveFormat(short)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(short, format));
    return;
}

void DiskCompressor::setPassword (const QString &passwd)
{
    getMethod(m_Compressor, "setPassword(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, passwd));
    return;
}

void DiskCompressor::setBlockSize(int size)
{
    getMethod(m_Compressor, "setBlockSize(int)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(int, size));
    return;
}

void DiskCompressor::addFiles(const QString &file)
{
    getMethod(m_Compressor, "addFiles(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString,file));
    return;
}

void DiskCompressor::addFiles(const QStringList &files)
{
    getMethod(m_Compressor, "addFiles(const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList, files));
    return;
}

void DiskCompressor::addFiles(const QString &entryName, const QString &file)
{
    getMethod(m_Compressor, "addFiles(const QString& , const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, entryName), Q_ARG(QString, file));
    return;
}

void DiskCompressor::addFiles(const QStringList &entryNames, const QStringList &files)
{
    getMethod(m_Compressor, "addFiles(const QStringList& , const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList,entryNames),Q_ARG(QStringList, files));
    return;
}

void DiskCompressor::removeFiles(const QString &file)
{
    getMethod(m_Compressor, "removeFiles(const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString,file));
    return;
}

void DiskCompressor::removeFiles(const QStringList &files)
{
    getMethod(m_Compressor, "removeFiles(const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList, files));
    return;
}

void DiskCompressor::removeFiles(const QString &entryName, const QString &file)
{
    getMethod(m_Compressor, "removeFiles(const QString& , const QString&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QString, entryName), Q_ARG(QString, file));
    return;
}

void DiskCompressor::removeFiles(const QStringList &entryNames, const QStringList &files)
{
    getMethod(m_Compressor, "removeFiles(const QStringList& , const QStringList&)")
    .invoke(m_Compressor.data(), Qt::QueuedConnection, Q_ARG(QStringList,entryNames),Q_ARG(QStringList, files));
    return;
}

void DiskCompressor::clear()
{
    getMethod(m_Compressor, "clear()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}

void DiskCompressor::start()
{
    getMethod(m_Compressor, "start()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}

void DiskCompressor::cancel()
{
    getMethod(m_Compressor, "cancel()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}

void DiskCompressor::pause()
{
    getMethod(m_Compressor, "pause()")
    .invoke(m_Compressor.data(),Qt::QueuedConnection);
    return;
}

void DiskCompressor::resume()
{
    getMethod(m_Compressor, "resume()")
    .invoke(m_Compressor.data(), Qt::QueuedConnection);
    return;
}
