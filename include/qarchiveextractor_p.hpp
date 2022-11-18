#ifndef QARCHIVE_EXTRACTOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_EXTRACTOR_PRIVATE_HPP_INCLUDED
#include <QIODevice>
#include <QBuffer>
#include <QObject>
#include <QString>
#include <QVector>
#include <QEventLoop>
#include <QStringList>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QJsonObject>
#include <QDir>

#include "qarchivememoryextractoroutput.hpp"
#include "qarchiveutils_p.hpp"

namespace QArchive {

class ArchiveFilter;

class MutableMemoryFile {
  public:
    MutableMemoryFile();
    ~MutableMemoryFile();

    void setFileInformation(const QJsonObject&);
    void setBuffer(QBuffer*);

    QJsonObject getFileInformation() const;
    QSharedPointer<QBuffer> getBuffer() const;
  private:
    QJsonObject m_FileInformation;
    QSharedPointer<QBuffer> m_Buffer;
};

class ExtractorPrivate : public QObject {
    Q_OBJECT
  public:
    ExtractorPrivate(bool memoryMode = false);
    ~ExtractorPrivate() override;
  public Q_SLOTS:
    void setArchive(QIODevice*);
    void setArchive(const QString&);
    void setBlockSize(int);
    void setCalculateProgress(bool);
    void setOutputDirectory(const QString&);
    void setPassword(const QString&);
    void addFilter(const QString&);
    void addFilter(const QStringList&);
    void addIncludePattern(const QString&);
    void addIncludePattern(const QStringList&);
    void addExcludePattern(const QString&);
    void addExcludePattern(const QStringList&);
    void setBasePath(const QString&);
    void clear();

    void getInfo();

    void start();
    void cancel();
    void pause();
    void resume();

  private Q_SLOTS:
    short openArchive();
    short checkOutputDirectory();
    short getTotalEntriesCount();
    short processArchiveInformation();
    short writeData(struct archive_entry*);
    short extract();
  Q_SIGNALS:
    void started();
    void canceled();
    void paused();
    void resumed();
    void diskFinished();
    void memoryFinished(MemoryExtractorOutput*);
    void error(short);
    void info(QJsonObject);
    void progress(QString, int, int, qint64, qint64);
    void getInfoRequirePassword(int);
    void extractionRequirePassword(int);
  private:
    bool b_MemoryMode = false,
         b_ProcessingArchive = false,
         b_StartRequested = false;

    bool b_PauseRequested = false,
         b_CancelRequested = false,
         b_Paused = false,
         b_Started = false,
         b_Finished = false,
         b_NoProgress = true,
         b_ArchiveOpened = false,
         b_QIODeviceOwned = false; // If set, free m_Archive on clear()
    int n_PasswordTriedCountGetInfo = 0,
        n_PasswordTriedCountExtract = 0,
        n_ProcessedEntries = 0,
        n_TotalEntries = -1,
        n_BlockSize = 10240,
        n_Flags = 0;
    qint64 n_BytesProcessed = 0,
           n_BytesTotal = 0;

    QString m_OutputDirectory,
            m_Password,
            m_ArchivePath;
    QIODevice *m_Archive = nullptr;
    archive_entry *m_CurrentArchiveEntry = nullptr;
    MutableMemoryFile m_CurrentMemoryFile;
    QSharedPointer<struct archive> m_ArchiveRead;
    QSharedPointer<struct archive> m_ArchiveWrite;
    QStringList m_ExtractFilters;
    QScopedPointer<QJsonObject> m_Info;
    QScopedPointer<QVector<MemoryFile>> m_ExtractedFiles;
    QSharedPointer<ArchiveFilter> m_archiveFilter;
    bool b_hasBasePath = false;
    QDir m_basePath;
};
}
#endif // QARCHIVE_EXTRACTOR_PRIVATE_HPP_INCLUDED
