#include <QArchiveMemoryExtractorTests.hpp>

QArchiveMemoryExtractorTests::QArchiveMemoryExtractorTests(QTemporaryDir *dir)
    : QObject(),
      QArchiveTestCases(dir) {
}

QArchiveMemoryExtractorTests::~QArchiveMemoryExtractorTests() {
}

void QArchiveMemoryExtractorTests::initTestCase() {
    QDir cases(TestCasesDir);
    if(cases.exists()) {
        if(!(
                    QFileInfo(TestCase1ArchivePath).exists() &&
                    QFileInfo(TestCase2ArchivePath).exists() &&
                    QFileInfo(TestCase3ArchivePath).exists() &&
                    QFileInfo(TestCase4ArchivePath).exists() &&
                    QFileInfo(TestCase5ArchivePath).exists() &&
                    QFileInfo(TestCase6ArchivePath).exists()
                )) {
            QFAIL("cannot find test case files.");
            return;
        }
    } else {

        QFAIL("cannnot find test cases.");
    }
}

void QArchiveMemoryExtractorTests::simpleExtraction() {
    QArchive::MemoryExtractor e(TestCase1ArchivePath);
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);

    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    QList<QVariant> output = spyInfo.takeFirst();
    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test1OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

void QArchiveMemoryExtractorTests::usingPauseResume() {
    QArchive::MemoryExtractor e(TestCase2ArchivePath);
    bool startedEmitted = false,
         pausedEmitted = false,
         resumedEmitted = false;
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);
    QObject::connect(&e, &QArchive::MemoryExtractor::started, [&]() {
        startedEmitted = true;
        e.pause();
        return;
    });
    QObject::connect(&e, &QArchive::MemoryExtractor::paused, [&]() {
        pausedEmitted = true;
        QTimer::singleShot(1000, &e, SLOT(resume()));
        return;
    });
    QObject::connect(&e, &QArchive::MemoryExtractor::resumed, [&]() {
        resumedEmitted = true;
        return;
    });

    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);
    e.start();

    QVERIFY(spyInfo.wait() || spyInfo.count());

    QVERIFY(startedEmitted);
    QVERIFY(pausedEmitted);
    QVERIFY(resumedEmitted);

    /* Verify output and contents. */
    QList<QVariant> output = spyInfo.takeFirst();
    QVERIFY(output.count() >= 1);

    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    QVERIFY(data->getFiles().count() >= 1);
    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test2OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

void QArchiveMemoryExtractorTests::usingExtractFilters() {
    /*
     * Note:
     * Only files present in the filter gets extracted.
    */
    QArchive::MemoryExtractor e(TestCase3ArchivePath);
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);

    e.addFilter(QFileInfo(Test3OutputFile2).fileName());
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    /* Test the output file and also the contents. */
    QList<QVariant> output = spyInfo.takeFirst();
    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    /// Test3OUtputFile1 should not exists
    QVERIFY(data->getFiles().count() == 1);
    QVERIFY(
        data->getFiles()
        .at(0)
        .fileInformation()
        .value("FileName")
        .toString() != QFileInfo(Test3OutputFile1).fileName());
    
    QVERIFY(
        data->getFiles()
        .at(0)
        .fileInformation()
        .value("FileName")
        .toString() == QFileInfo(Test3OutputFile2).fileName());

    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test3Output2Contents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

void QArchiveMemoryExtractorTests::usingPassword() {
    QArchive::MemoryExtractor e(TestCase4ArchivePath);
    e.setPassword(Test4Password);
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);

    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    QList<QVariant> output = spyInfo.takeFirst();
    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test4OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

void QArchiveMemoryExtractorTests::informationExtraction() {
    QArchive::MemoryExtractor e(TestCase5ArchivePath);
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);

    QSignalSpy spyInfo(&e, SIGNAL(info(QJsonObject)));
    e.getInfo();
    QVERIFY(spyInfo.wait() || spyInfo.count());
}

void QArchiveMemoryExtractorTests::testInvalidArchivePath() {
    QArchive::MemoryExtractor e("THISDOESNOTEXISTS");
    QSignalSpy spyInfo(&e, SIGNAL(error(short)));
    e.start();
    QVERIFY(spyInfo.wait() || spyInfo.count());
}

void QArchiveMemoryExtractorTests::runningExtractorNonSingleThreaded() {
    QArchive::MemoryExtractor e(TestCase1ArchivePath,
                                /*parent=*/nullptr, /*singleThread=*/false);
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    /* Test the output file and also the contents. */
    QList<QVariant> output = spyInfo.takeFirst();
    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test1OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

void QArchiveMemoryExtractorTests::extractArchiveFromQIODevice() {
    QFile file(TestCase1ArchivePath);
    QVERIFY(file.open(QIODevice::ReadOnly) == true);

    QArchive::MemoryExtractor e(&file);
    e.setOutputDirectory(TestCase1OutputDir);

    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    /* Test the output file and also the contents. */
    QList<QVariant> output = spyInfo.takeFirst();
    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test1OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

/* Sometimes , if the skip callback is not proper , the getInfo will not
 * work for tar archives so we need to test it everytime. */
void QArchiveMemoryExtractorTests::getInfoFromTarArchiveWithNoFilters() {
    QArchive::MemoryExtractor e(TestCase6ArchivePath);
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);

    QSignalSpy spyInfo(&e, SIGNAL(info(QJsonObject)));
    e.getInfo();
    QVERIFY(spyInfo.wait() || spyInfo.count());
}

void QArchiveMemoryExtractorTests::extractTarArchiveWithNoFilters() {
    QArchive::MemoryExtractor e(TestCase6ArchivePath);;
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);

    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);
    e.start();

    QVERIFY(spyInfo.wait() || spyInfo.count() == 1);

    /// Verify output
    QList<QVariant> output = spyInfo.takeFirst();
    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test6OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

void QArchiveMemoryExtractorTests::isExtractorObjectReuseable() {
    QArchive::MemoryExtractor e(TestCase5ArchivePath);
    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);

    QFile TestOutput;
    QSignalSpy spyInfo(&e, &QArchive::MemoryExtractor::finished);
    e.start();

    QVERIFY(spyInfo.wait() || spyInfo.count() == 1);

    /// Verify output of first archive
    QList<QVariant> output = spyInfo.takeFirst();
    auto data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    auto buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test5OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;

    e.setArchive(TestCase4ArchivePath);
    e.setPassword(Test4Password);
    e.start();

    QVERIFY(spyInfo.wait() || spyInfo.count() == 2);

    output = spyInfo.takeFirst();
    data = output.at(0).value<QArchive::MemoryExtractorOutput*>();

    buffer = data->getFiles().at(0).buffer();

    buffer->open(QIODevice::ReadOnly);
    QCOMPARE(Test4OutputContents, QString(buffer->readAll()));
    buffer->close();

    delete data;
}

void QArchiveMemoryExtractorTests::testProgress() {
    QArchive::MemoryExtractor e(TestCase3ArchivePath);
    e.setCalculateProgress(true);
    e.setBlockSize(4);

    QObject::connect(&e, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryExtractorTests::defaultErrorHandler);
    QSignalSpy finishedSpyInfo(&e, &QArchive::MemoryExtractor::finished);
    QSignalSpy progressSpyInfo(&e, SIGNAL(progress(QString, int, int, qint64, qint64)));

    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(finishedSpyInfo.wait() || finishedSpyInfo.count());

    QCOMPARE(progressSpyInfo.count(), 4);
}

void QArchiveMemoryExtractorTests::cleanupTestCase() {
    QDir dir(TestOutputDir);
    dir.removeRecursively();
}

void QArchiveMemoryExtractorTests::defaultErrorHandler(short code) {
    auto scode = QString::number(code);
    scode.prepend("error:: ");
    QFAIL(QTest::toString(scode));
    return;
}
