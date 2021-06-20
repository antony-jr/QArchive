#ifndef QARCHIVE_COMPRESSOR_HPP_INCLUDED
#define QARCHIVE_COMPRESSOR_HPP_INCLUDED
#include <QObject>
#include <QBuffer>
#include <QString>
#include <QVariant>
#include <QStringList>
#include <QScopedPointer>
#include <QThread>

#include "qarchive_global.hpp"

namespace QArchive {

class CompressorPrivate;
class QARCHIVE_EXPORT Compressor : public QObject {
    Q_OBJECT
  public:
    Compressor(bool memoryMode = false, QObject *parent = nullptr, bool singleThreaded = true);
    ~Compressor();
  public Q_SLOTS:
    void setFileName(const QString&);
    void setArchiveFormat(short);
    void setPassword(const QString&);
    void setBlockSize(int);
    void addFiles(const QString&, QIODevice*);
    void addFiles(const QStringList&, const QVariantList&);
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
    void memoryFinished(QBuffer*);
    void diskFinished();

  private:
    QScopedPointer<CompressorPrivate> m_Compressor;
    QScopedPointer<QThread> m_Thread;
};
}
#endif // QARCHIVE_COMPRESSOR_HPP_INCLUDED
