#ifndef QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
#include <QScopedPointer>
#include <QFile>
#include <QString>
#include <QObject>
#include <QThread>
#include <QJsonObject>

namespace QArchive
{
class DiskExtractorPrivate;
class DiskExtractor : public QObject
{
    Q_OBJECT
public:
    DiskExtractor(bool singleThreaded = true , QObject *parent = nullptr);
    DiskExtractor(QFile*, bool singleThreaded = true , QObject *parent = nullptr);
    DiskExtractor(const QString&, bool singleThreaded = true , QObject *parent = nullptr);
    DiskExtractor(const QString& , const QString& , bool singleThreaded = true , QObject *parent = nullptr);
    ~DiskExtractor();

public Q_SLOTS:
    void setArchive(QFile*);
    void setArchive(const QString&);
    void setArchive(const QString& , const QString&);
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
    QScopedPointer<DiskExtractorPrivate> _mExtractor;
    QScopedPointer<QThread> _mThread;
};
}
#endif // QARCHIVE_DISK_EXTRACTOR_HPP_INCLUDED
