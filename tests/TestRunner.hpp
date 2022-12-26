#ifndef TEST_RUNNER_HPP_INCLUDED
#define TEST_RUNNER_HPP_INCLUDED
#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QTemporaryDir>

#include "QArchiveDiskCompressorTests.hpp"
#include "QArchiveDiskExtractorTests.hpp"
#include "QArchiveMemoryCompressorTests.hpp"
#include "QArchiveMemoryExtractorTests.hpp"

class TestRunner : public QObject {
    Q_OBJECT
  public:
    TestRunner();
  public Q_SLOTS:
    void start();
  private Q_SLOTS:
    void runTests();
  Q_SIGNALS:
    void finished();
  private:
    QTemporaryDir m_TempDir;
    QFuture<void> m_Future;
    QFutureWatcher<void> m_FutureWatcher;
    std::unique_ptr<QArchiveDiskCompressorTests> m_CompressorTests;
    std::unique_ptr<QArchiveMemoryCompressorTests> m_MemoryCompressorTests;
    std::unique_ptr<QArchiveDiskExtractorTests> m_ExtractorTests;
    std::unique_ptr<QArchiveMemoryExtractorTests> m_MemoryExtractorTests;
};
#endif
