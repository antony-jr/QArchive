#ifndef QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
#include <QScopedPointer>
#include <QSharedPointer>
#include <QBuffer>
#include <QIODevice>
#include <QString>
#include <QObject>
#include <QThread>
#include <QJsonObject>

#include "qarchive_global.hpp"

namespace QArchive {
class ExtractorPrivate;
class QARCHIVE_EXPORT MemoryExtractor : public QObject {
    Q_OBJECT
  public:
    MemoryExtractor(QObject *parent = nullptr, bool singleThreaded = true);
    MemoryExtractor(QIODevice*, QObject *parent = nullptr, bool singleThreaded = true);
    MemoryExtractor(const QString&, QObject *parent = nullptr, bool singleThreaded = true);
    ~MemoryExtractor();
  public Q_SLOTS:
    void setArchive(QIODevice*);
    void setArchive(const QString&);
    void setBlockSize(int);
    void setCalculateProgress(bool);
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
    void finished(QVector<QPair<QJsonObject, QSharedPointer<QBuffer>>>*);
    void error(short);
    void progress(QString, int, int, qint64, qint64);
    void getInfoRequirePassword(int);
    void extractionRequirePassword(int);
    void info(QJsonObject);
  
  private:
    QScopedPointer<ExtractorPrivate> m_Extractor;
    QScopedPointer<QThread> m_Thread;
};
}
#endif // QARCHIVE_MEMORY_EXTRACTOR_HPP_INCLUDED
