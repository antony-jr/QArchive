#ifndef QARCHIVE_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_EXTRACTOR_HPP_INCLUDED
#include <QScopedPointer>
#include <QIODevice>
#include <QBuffer>
#include <QString>
#include <QObject>
#include <QThread>
#include <QJsonObject>

#include "qarchivememoryextractoroutput.hpp"
#include "qarchive_global.hpp"

namespace QArchive {
class ExtractorPrivate;
class QARCHIVE_EXPORT Extractor : public QObject {
    Q_OBJECT
  public:
    Extractor(bool memoryMode = false, QObject *parent = nullptr, bool singleThreaded = true);
    ~Extractor();
  public Q_SLOTS:
    void setArchive(QIODevice*);
    void setArchive(const QString&);
    void setArchive(const QString&, const QString&);
    void setBlockSize(int);
    void setCalculateProgress(bool);
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
    void diskFinished();
    void memoryFinished(MemoryExtractorOutput*);
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
#endif // QARCHIVE_EXTRACTOR_HPP_INCLUDED
