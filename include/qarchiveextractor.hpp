#ifndef QARCHIVE_EXTRACTOR_HPP_INCLUDED
#define QARCHIVE_EXTRACTOR_HPP_INCLUDED
#include <QBuffer>
#include <QIODevice>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QThread>

#include <memory>

#include "qarchive_global.hpp"
#include "qarchivememoryextractoroutput.hpp"

namespace QArchive {
class ExtractorPrivate;
class QARCHIVE_EXPORT Extractor : public QObject {
    Q_OBJECT
public:
    Q_DISABLE_COPY(Extractor)
    Extractor(bool memoryMode = false, QObject* parent = nullptr, bool singleThreaded = true);
    ~Extractor() override;
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
} // namespace QArchive
#endif // QARCHIVE_EXTRACTOR_HPP_INCLUDED
