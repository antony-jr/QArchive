#include <TestRunner.hpp>
#include <QMetaObject>
#include <QMetaMethod>
#include <QtConcurrentRun>
#include <QTest>

TestRunner::TestRunner()
    : QObject() {
    m_TempDir.reset(new QTemporaryDir);
    m_Future.reset(new QFuture<void>);
    m_FutureWatcher.reset(new QFutureWatcher<void>);
    m_CompressorTests.reset(new QArchiveDiskCompressorTests(m_TempDir.data()));
    m_ExtractorTests.reset(new QArchiveDiskExtractorTests(m_TempDir.data()));
    m_MemoryExtractorTests.reset(new QArchiveMemoryExtractorTests(m_TempDir.data()));
    m_MemoryCompressorTests.reset(new QArchiveMemoryCompressorTests(m_TempDir.data()));

    connect(m_FutureWatcher.data(), &QFutureWatcher<void>::finished,
            this, &TestRunner::finished, Qt::DirectConnection);
}

TestRunner::~TestRunner() {
}

void TestRunner::start() {
    *(m_Future.data()) = QtConcurrent::run([&]() {
        runTests();
        return;
    });
    m_FutureWatcher->setFuture(*(m_Future.data()));

}

void TestRunner::runTests() {
    // Run the compressor tests which should generate the
    // desired archives to test it with the extractor.
    QTest::qExec(m_CompressorTests.data());

    // Run disk extractor tests.
    QTest::qExec(m_ExtractorTests.data());

    // Run memory extractor tests
    QTest::qExec(m_MemoryExtractorTests.data());

    // Run memory compressor tests
    QTest::qExec(m_MemoryCompressorTests.data());
}
