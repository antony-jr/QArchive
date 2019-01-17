#ifndef TEST_RUNNER_HPP_INCLUDED
#define TEST_RUNNER_HPP_INCLUDED
#include <QObject>
#include <QFuture>
#include <QFutureWatcher>
#include <QScopedPointer>
#include <QArchiveDiskExtractorTests.hpp>

class TestRunner : public QObject
{
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
    QScopedPointer<QFuture<void>> m_Future;
    QScopedPointer<QFutureWatcher<void>> m_FutureWatcher;
    QScopedPointer<QArchiveDiskExtractorTests> m_ExtractorTests;
};
#endif
