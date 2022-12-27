#ifndef QARCHIVE_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_EXTRACTOR_HPP_INCLUDED
#include <QIODevice>
#include <QBuffer>
#include <QString>
#include <QObject>
#include <QThread>
#include <QJsonObject>

#include <memory>

#include "qarchivememoryextractoroutput.hpp"
#include "qarchive_global.hpp"

namespace QArchive {
class ExtractorPrivate;
class QARCHIVE_EXPORT Extractor : public QObject {
    Q_OBJECT
  public:
    Extractor(bool memoryMode = false, QObject *parent = nullptr, bool singleThreaded = true);
    ~Extractor() override;
    Extractor(const Extractor&) = delete;
    Extractor& operator=(const Extractor&) = delete;
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
    void addIncludePattern(const QString&);
    void addIncludePattern(const QStringList&);
    void addExcludePattern(const QString&);
    void addExcludePattern(const QStringList&);
    void setBasePath(const QString&);
    void setRawMode(bool);
    void setRawOutputFilename(const QString&);
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
    std::unique_ptr<ExtractorPrivate> m_Extractor;
    std::unique_ptr<QThread> m_Thread;
};
}  // namespace QArchive
#endif // QARCHIVE_EXTRACTOR_HPP_INCLUDED
