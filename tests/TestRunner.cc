#include <TestRunner.hpp>
#include <QMetaObject>
#include <QMetaMethod>
#include <QtConcurrentRun>
#include <QTest>

TestRunner::TestRunner()
    : QObject()
{
    m_Future.reset(new QFuture<void>);
    m_FutureWatcher.reset(new QFutureWatcher<void>);
    m_ExtractorTests.reset(new QArchiveDiskExtractorTests);

    connect(m_FutureWatcher.data(), &QFutureWatcher<void>::finished,
            this, &TestRunner::finished, Qt::DirectConnection);
}

TestRunner::~TestRunner()
{
}

void TestRunner::start()
{
    *(m_Future.data()) = QtConcurrent::run([&]() {
        runTests();
        return;
    });
    m_FutureWatcher->setFuture(*(m_Future.data()));

}

void TestRunner::runTests()
{
    /* Run extractor tests. */
    QTest::qExec(m_ExtractorTests.data());
}
