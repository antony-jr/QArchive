#ifndef QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#include <QFile>
#include <QObject>
#include <QString>
#include <QEventLoop>
#include <QStringList>
#include <QSharedPointer>
#include <QJsonObject>

struct archive;
struct archive_entry;

namespace QArchive
{
class DiskExtractorPrivate : public QObject
{
    Q_OBJECT
public:
    DiskExtractorPrivate();
    DiskExtractorPrivate(QFile*);
    DiskExtractorPrivate(const QString&);
    ~DiskExtractorPrivate();
public Q_SLOTS:
    void setArchive(QFile*);
    void setArchive(const QString&);
    void setBlockSize(int);
    void setShowProgress(bool);
    void setOutputDirectory(const QString&);
    void setPassword(const QString&);
    void addFilter(const QString&);
    void addFilter(const QStringList&);
    void clear();

    void getInfo();

    void start();
    void cancel();
    void pause();
    void resume();

private Q_SLOTS:
    short getTotalEntriesCount();
    short processArchiveInformation();
    short writeData(struct archive_entry*);
    short extract();
Q_SIGNALS:
    void started();
    void canceled();
    void paused();
    void resumed();
    void finished();
    void error(short);
    void progress(QString, int, int, int);
    void getInfoRequirePassword(int);
    void extractionRequirePassword(int);
    void info(QJsonObject);

private:
    bool _bPauseRequested = false,
         _bCancelRequested = false,
         _bPaused = false,
         _bStarted = false,
         _bFinished = false,
         _bNoProgress = false;
    int _nPasswordTriedCountGetInfo = 0,
        _nPasswordTriedCountExtract = 0,
        _nProcessedEntries = 0,
        _nTotalEntries = -1,
        _nBlockSize = 10240,
        _nFlags = 0;
    QString _mOutputDirectory,
            _mPassword;
    QSharedPointer<struct archive> _mArchiveRead = nullptr,
                                       _mArchiveWrite = nullptr;
    QSharedPointer<QJsonObject> _mInfo;
    QSharedPointer<QStringList> _mExtractFilters = nullptr;
    QSharedPointer<QFile> _mArchive = nullptr;
};
}
#endif // QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
