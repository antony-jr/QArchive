#ifndef QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
#define QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QStringList>
#include <QScopedPointer>
#include <QThread>

namespace QArchive {

class DiskCompressorPrivate;
class DiskCompressor : public QObject {
    Q_OBJECT
  public:
    DiskCompressor(QObject *parent = nullptr, bool singleThreaded = true);
    DiskCompressor(const QString&, QObject *parent = nullptr,  bool singleThreaded = true);
    DiskCompressor(const QString&, short, QObject *parent = nullptr, bool singleThreaded = true);
    ~DiskCompressor();
  public Q_SLOTS:
    void setFileName(const QString&);
    void setArchiveFormat(short);
    void setPassword(const QString&);
    void setBlockSize(int);
    void addFiles(const QString&);
    void addFiles(const QStringList&);
    void addFiles(const QString&, const QString&);
    void addFiles(const QStringList&, const QStringList&);
    void removeFiles(const QString&);
    void removeFiles(const QStringList&);
    void removeFiles(const QString&, const QString&);
    void removeFiles(const QStringList&, const QStringList&);
    void clear();

    void start();
    void cancel();
    void pause();
    void resume();

  Q_SIGNALS:
    void progress(QString, int, int, qint64, qint64);
    void error(short, QString);
    void started();
    void canceled();
    void paused();
    void resumed();
    void finished();

  private:
    QScopedPointer<DiskCompressorPrivate> m_Compressor;
    QScopedPointer<QThread> m_Thread;
};
}
#endif // QARCHIVE_DISK_COMPRESSOR_HPP_INCLUDED
