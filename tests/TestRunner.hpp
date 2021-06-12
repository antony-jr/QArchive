#ifndef TEST_RUNNER_HPP_INCLUDED
#define TEST_RUNNER_HPP_INCLUDED
#include <QObject>
#include <QTemporaryDir>
#include <QFuture>
#include <QFutureWatcher>
#include <QScopedPointer>
#include <QArchiveDiskExtractorTests.hpp>
#include <QArchiveMemoryExtractorTests.hpp>
#include <QArchiveDiskCompressorTests.hpp>

class TestRunner : public QObject {
    Q_OBJECT
  public:
    TestRunner();
    ~TestRunner();
  public Q_SLOTS:
    void start();
  private Q_SLOTS:
    void runTests();
  Q_SIGNALS:
    void finished();
  private:
    QScopedPointer<QTemporaryDir> m_TempDir;
    QScopedPointer<QFuture<void>> m_Future;
    QScopedPointer<QFutureWatcher<void>> m_FutureWatcher;
    QScopedPointer<QArchiveDiskCompressorTests> m_CompressorTests;
    QScopedPointer<QArchiveDiskExtractorTests> m_ExtractorTests;
    QScopedPointer<QArchiveMemoryExtractorTests> m_MemoryExtractorTests;
};
#endif
