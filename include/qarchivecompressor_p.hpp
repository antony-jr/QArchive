#ifndef QARCHIVE_COMPRESSOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_COMPRESSOR_PRIVATE_HPP_INCLUDED
#include <QBuffer>
#include <QFile>
#include <QObject>
#include <QSaveFile>
#include <QSharedPointer>
#include <QString>
#include <QStringList>
#include <QVariantList>

#include <deque>
#include <memory>

#include "qarchiveutils_p.hpp"

namespace QArchive {
class CompressorPrivate : public QObject {
    Q_OBJECT
public:
    Q_DISABLE_COPY(CompressorPrivate)
    explicit CompressorPrivate(bool memoryMode = false);
    ~CompressorPrivate() override;
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
    void memoryFinished(QBuffer*);
    void diskFinished();

public:
    struct Node {
        short open();

        QString path, entry;
        QIODevice* io = nullptr;
        bool valid = false;
        bool isInMemory = false;
    };

private:
    bool b_MemoryMode = false;
    bool b_PauseRequested = false;
    bool b_CancelRequested = false;
    bool b_Paused = false;
    bool b_Started = false;
    bool b_Finished = false;

    QString m_Password; /* Only used for ZIP. */
    short m_ArchiveFormat = 0; /* Defaults to ZIP. */
    int n_BlockSize = 10240;
    int n_TotalEntries = 0;
    qint64 n_BytesProcessed = 0;
    qint64 n_BytesTotal = 0;
    QSharedPointer<struct archive> m_ArchiveWrite;
    std::unique_ptr<QSaveFile> m_TemporaryFile;
    std::unique_ptr<QBuffer> m_Buffer;
    std::deque<Node*> m_ConfirmedFiles;
    std::deque<Node*> m_StaggedFiles;
};
} // namespace QArchive
#endif // QARCHIVE_COMPRESSOR_PRIVATE_HPP_INCLUDED
