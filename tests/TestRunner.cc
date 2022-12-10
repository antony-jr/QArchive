#include <TestRunner.hpp>
#include <QMetaObject>
#include <QMetaMethod>
#include <QtConcurrentRun>
#include <QTest>

TestRunner::TestRunner() {
    m_CompressorTests.reset(new QArchiveDiskCompressorTests(&m_TempDir));
    m_ExtractorTests.reset(new QArchiveDiskExtractorTests(&m_TempDir));
    m_MemoryExtractorTests.reset(new QArchiveMemoryExtractorTests(&m_TempDir));
    m_MemoryCompressorTests.reset(new QArchiveMemoryCompressorTests(&m_TempDir));

    connect(&m_FutureWatcher, &QFutureWatcher<void>::finished,
            this, &TestRunner::finished, Qt::DirectConnection);
}

TestRunner::~TestRunner() = default;

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
    QTest::qExec(m_CompressorTests.get());

    // Run disk extractor tests.
    QTest::qExec(m_ExtractorTests.get());

    // Run memory extractor tests
    QTest::qExec(m_MemoryExtractorTests.get());

    // Run memory compressor tests
    QTest::qExec(m_MemoryCompressorTests.get());
}
