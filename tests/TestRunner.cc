#include <QMetaMethod>
#include <QMetaObject>
#include <QTest>
#include <QtConcurrentRun>
#include <TestRunner.hpp>

TestRunner::TestRunner()
    : m_CompressorTests(&m_TempDir),
      m_MemoryCompressorTests(&m_TempDir),
      m_ExtractorTests(&m_TempDir),
      m_MemoryExtractorTests(&m_TempDir) {
  connect(&m_FutureWatcher, &QFutureWatcher<void>::finished,
          [&]() { emit finished(n_ReturnCode); });
}

void TestRunner::start() {
  m_Future = QtConcurrent::run([this] {
    runTests();
    return;
  });
  m_FutureWatcher.setFuture(m_Future);
}

void TestRunner::runTests() {
  // Run the compressor tests which should generate the
  // desired archives to test it with the extractor. 
  n_ReturnCode |= QTest::qExec(&m_CompressorTests);
  n_ReturnCode |= QTest::qExec(&m_ExtractorTests);
  n_ReturnCode |= QTest::qExec(&m_MemoryExtractorTests);
  n_ReturnCode |= QTest::qExec(&m_MemoryCompressorTests);
}
