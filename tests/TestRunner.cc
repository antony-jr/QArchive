#include <TestRunner.hpp>
#include <QMetaObject>
#include <QMetaMethod>
#include <QtConcurrentRun>
#include <QTest>

TestRunner::TestRunner() {
    m_TempDir.reset(new QTemporaryDir);
    m_Future.reset(new QFuture<void>);
    m_FutureWatcher.reset(new QFutureWatcher<void>);
    m_CompressorTests.reset(new QArchiveDiskCompressorTests(m_TempDir.get()));
    m_ExtractorTests.reset(new QArchiveDiskExtractorTests(m_TempDir.get()));
    m_MemoryExtractorTests.reset(new QArchiveMemoryExtractorTests(m_TempDir.get()));
    m_MemoryCompressorTests.reset(new QArchiveMemoryCompressorTests(m_TempDir.get()));

    connect(m_FutureWatcher.get(), &QFutureWatcher<void>::finished,
            this, &TestRunner::finished, Qt::DirectConnection);
}

TestRunner::~TestRunner() = default;

void TestRunner::start() {
    *m_Future = QtConcurrent::run([this] {
        runTests();
        return;
    });
    m_FutureWatcher->setFuture(*m_Future);

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
