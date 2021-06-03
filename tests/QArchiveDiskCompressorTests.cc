#include <QArchiveDiskCompressorTests.hpp>

QArchiveDiskCompressorTests::QArchiveDiskCompressorTests(QTemporaryDir *dir)
    : QObject(),
      QArchiveTestCases(dir) { }

QArchiveDiskCompressorTests::~QArchiveDiskCompressorTests() { }

void QArchiveDiskCompressorTests::initTestCase() {
    if(TestCasesDir.isEmpty()) {
        QFAIL("Temporary directory failed to construct.");
    }
    QDir dir;
    dir.mkpath(TestCase1OutputDir);
    dir.mkpath(TestCase2OutputDir);
    dir.mkpath(TestCase3OutputDir);
    dir.mkpath(TestCase4OutputDir);
    dir.mkpath(TestCase5OutputDir);
    dir.mkpath(TestCase6OutputDir);
    return;
}

void QArchiveDiskCompressorTests::simpleCompression() {
    QArchive::DiskCompressor e(TestCase1ArchivePath);

    /* Write the file to compress and add it. */
    QFile TestOutput(TemporaryFilePath);
    QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
    TestOutput.write(Test1OutputContents.toLatin1());
    TestOutput.close();

    e.addFiles(/*entry name(optional)=*/QFileInfo(Test1OutputFile).fileName(), TemporaryFilePath);

    QObject::connect(&e, &QArchive::DiskCompressor::error,
                     this, &QArchiveDiskCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&e, SIGNAL(finished()));
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    /* The archive should also exists. */
    QVERIFY(QFileInfo::exists(TestCase1ArchivePath));

    /*
     * Note:
     * The archives will be later tested by the extractor tests to
     * verify if it is indeed authentic archives by cross checking the
     * output files.
    */
}

void QArchiveDiskCompressorTests::usingPauseResume() {
    QArchive::DiskCompressor e(TestCase2ArchivePath);

    QFile TestOutput(TemporaryFilePath);
    QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
    TestOutput.write(Test2OutputContents.toLatin1());
    TestOutput.close();

    e.addFiles(QFileInfo(Test2OutputFile).fileName(), TemporaryFilePath);

    bool startedEmitted = false,
         pausedEmitted = false,
         resumedEmitted = false;
    QObject::connect(&e, &QArchive::DiskCompressor::error,
                     this, &QArchiveDiskCompressorTests::defaultErrorHandler);
    QObject::connect(&e, &QArchive::DiskCompressor::started, [&]() {
        startedEmitted = true;
        e.pause();
        return;
    });
    QObject::connect(&e, &QArchive::DiskCompressor::paused, [&]() {
        pausedEmitted = true;
        QTimer::singleShot(1000, &e, SLOT(resume()));
        return;
    });
    QObject::connect(&e, &QArchive::DiskCompressor::resumed, [&]() {
        resumedEmitted = true;
        return;
    });

    QSignalSpy spyInfo(&e, SIGNAL(finished()));
    e.start();

    QVERIFY(spyInfo.wait() || spyInfo.count());

    QVERIFY(startedEmitted);
    QVERIFY(pausedEmitted);
    QVERIFY(resumedEmitted);

    QVERIFY(QFileInfo::exists(TestCase2ArchivePath));
}

void QArchiveDiskCompressorTests::compressingMultipleFiles() {
    QArchive::DiskCompressor e(TestCase3ArchivePath);

    QFile TestOutput(TemporaryFilePath);
    QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
    TestOutput.write(Test3Output1Contents.toLatin1());
    TestOutput.close();

    TestOutput.setFileName(TemporaryFilePath + ".v2");
    QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
    TestOutput.write(Test3Output2Contents.toLatin1());
    TestOutput.close();


    e.addFiles(QStringList() << QFileInfo(Test3OutputFile1).fileName()
               << QFileInfo(Test3OutputFile2).fileName()
               ,
               QStringList() << TemporaryFilePath
               << (TemporaryFilePath + ".v2"));

    QObject::connect(&e, &QArchive::DiskCompressor::error,
                     this, &QArchiveDiskCompressorTests::defaultErrorHandler);
    QSignalSpy finishedSpyInfo(&e, SIGNAL(finished()));
    QSignalSpy progressSpyInfo(&e, SIGNAL(progress(QString, int, int, qint64, qint64)));
    e.start();

    QVERIFY(finishedSpyInfo.wait() || finishedSpyInfo.count());
    QVERIFY(QFileInfo::exists(TestCase3ArchivePath));

    QCOMPARE(progressSpyInfo.count(), 4);
}

void QArchiveDiskCompressorTests::encryptingZipArchive() {
    QArchive::DiskCompressor e(TestCase4ArchivePath);
    QFile TestOutput(TemporaryFilePath);
    QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
    TestOutput.write(Test4OutputContents.toLatin1());
    TestOutput.close();

    e.addFiles(QFileInfo(Test4OutputFile).fileName(), TemporaryFilePath);

    QObject::connect(&e, &QArchive::DiskCompressor::error,
                     this, &QArchiveDiskCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&e, SIGNAL(finished()));

    e.setPassword(Test4Password); /* Set password (Only works for Zip Format). */
    e.start();

    QVERIFY(spyInfo.wait() || spyInfo.count());
    QVERIFY(QFileInfo::exists(TestCase4ArchivePath));
}

void QArchiveDiskCompressorTests::runningCompressorNonSingleThreaded() {
    QArchive::DiskCompressor e(TestCase5ArchivePath, /*parent=*/ nullptr, /*singleThreaded=*/ false);
    QFile TestOutput(TemporaryFilePath);
    QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
    TestOutput.write(Test5OutputContents.toLatin1());
    TestOutput.close();

    e.addFiles(QFileInfo(Test5OutputFile).fileName(), TemporaryFilePath);

    QObject::connect(&e, &QArchive::DiskCompressor::error,
                     this, &QArchiveDiskCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&e, SIGNAL(finished()));
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    /* The archive should also exists. */
    QVERIFY(QFileInfo::exists(TestCase5ArchivePath));
}

void QArchiveDiskCompressorTests::compressingTarArchiveWithoutFilters() {
    QArchive::DiskCompressor e(TestCase6ArchivePath);

    /* Write the file to compress and add it. */
    QFile TestOutput(TemporaryFilePath);
    QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
    TestOutput.write(Test6OutputContents.toLatin1());
    TestOutput.close();

    e.addFiles(/*entry name(optional)=*/QFileInfo(Test6OutputFile).fileName(), TemporaryFilePath);

    QObject::connect(&e, &QArchive::DiskCompressor::error,
                     this, &QArchiveDiskCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&e, SIGNAL(finished()));
    e.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    /* The archive should also exists. */
    QVERIFY(QFileInfo::exists(TestCase6ArchivePath));
}

void QArchiveDiskCompressorTests::defaultErrorHandler(short code, QString file) {
    auto scode = QString::number(code);
    scode.prepend(("error::" + file) + ":: ");
    QFAIL(QTest::toString(scode));
    return;
}
