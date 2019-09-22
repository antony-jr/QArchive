#ifndef QARCHIVE_DISK_COMPRESSOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_DISK_COMPRESSOR_PRIVATE_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QPair>
#include <QSaveFile>
#include <QLinkedList>
#include <QScopedPointer>
#include <QSharedPointer>
#include <qarchiveutils_p.hpp>

namespace QArchive {
class DiskCompressorPrivate : public QObject {
    Q_OBJECT
  public:
    DiskCompressorPrivate();
    ~DiskCompressorPrivate();
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

  private Q_SLOTS:
    bool guessArchiveFormat();
    bool confirmFiles();
    short compress();

  Q_SIGNALS:
    void progress(QString, int, int, qint64, qint64);
    void error(short, QString);
    void started();
    void canceled();
    void paused();
    void resumed();
    void finished();

  private:
    bool b_PauseRequested = false,
         b_CancelRequested = false,
         b_Paused = false,
         b_Started = false,
         b_Finished = false;

    QString m_Password; /* Only used for ZIP. */
    short m_ArchiveFormat = 0; /* Defaults to ZIP. */
    int n_BlockSize = 10240,
        n_TotalEntries = 0;
    qint64 n_BytesProcessed = 0,
           n_BytesTotal = 0;
    QSharedPointer<struct archive> m_ArchiveWrite;
    QScopedPointer<QSaveFile> m_TemporaryFile;
    QScopedPointer<QLinkedList<QPair<QString, QString>>> m_ConfirmedFiles;
    QScopedPointer<QLinkedList<QPair<QString, QString>>> m_StaggedFiles;
};
}
#endif // QARCHIVE_DISK_COMPRESSOR_PRIVATE_HPP_INCLUDED
