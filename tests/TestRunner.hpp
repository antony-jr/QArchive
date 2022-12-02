#ifndef TEST_RUNNER_HPP_INCLUDED
#define TEST_RUNNER_HPP_INCLUDED
#include <QObject>
#include <QTemporaryDir>
#include <QFuture>
#include <QFutureWatcher>

#include "QArchiveDiskExtractorTests.hpp"
#include "QArchiveDiskCompressorTests.hpp"
#include "QArchiveMemoryExtractorTests.hpp"
#include "QArchiveMemoryCompressorTests.hpp"

class TestRunner : public QObject {
    Q_OBJECT
  public:
    TestRunner();
    ~TestRunner() override;
  public Q_SLOTS:
    void start();
  private Q_SLOTS:
    void runTests();
  Q_SIGNALS:
    void finished();
  private:
    std::unique_ptr<QTemporaryDir> m_TempDir;
    std::unique_ptr<QFuture<void>> m_Future;
    std::unique_ptr<QFutureWatcher<void>> m_FutureWatcher;
    std::unique_ptr<QArchiveDiskCompressorTests> m_CompressorTests;
    std::unique_ptr<QArchiveMemoryCompressorTests> m_MemoryCompressorTests;
    std::unique_ptr<QArchiveDiskExtractorTests> m_ExtractorTests;
    std::unique_ptr<QArchiveMemoryExtractorTests> m_MemoryExtractorTests;
};
#endif
