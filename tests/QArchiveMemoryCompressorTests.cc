#include "QArchiveMemoryCompressorTests.hpp"

QArchiveMemoryCompressorTests::QArchiveMemoryCompressorTests(QTemporaryDir *dir)
    : QObject(),
      QArchiveTestCases(dir) { }

QArchiveMemoryCompressorTests::~QArchiveMemoryCompressorTests() { }

void QArchiveMemoryCompressorTests::initTestCase() {
    return;
}

void QArchiveMemoryCompressorTests::simpleCompression() {
    QArchive::MemoryCompressor compressor(QArchive::SevenZipFormat);

    /* Write the file to compress and add it. */
    auto array = Test1OutputContents.toLocal8Bit();
    QBuffer buffer(&array);
    compressor.addFiles(/*entry name=*/QFileInfo(Test1OutputFile).fileName(), (QIODevice*)&buffer);

    QObject::connect(&compressor, &QArchive::MemoryCompressor::error,
                     this, &QArchiveMemoryCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&compressor, &QArchive::MemoryCompressor::finished);
    compressor.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    QList<QVariant> output = spyInfo.takeFirst();
    QVERIFY(output.count() == 1);
    auto archive = output.at(0).value<QBuffer*>();

    QArchive::MemoryExtractor extractor((QIODevice*)archive);
    QObject::connect(&extractor, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryCompressorTests::defaultExtractorErrorHandler);

    QSignalSpy spyExtractor(&extractor, &QArchive::MemoryExtractor::finished);
    extractor.start();

    QVERIFY(spyExtractor.wait() || spyExtractor.count());

    QList<QVariant> extractorOutput = spyExtractor.takeFirst();

    QVERIFY(extractorOutput.count() == 1);
    auto data = extractorOutput.at(0).value<QArchive::MemoryExtractorOutput*>();

    QVERIFY(data->getFiles().count() >= 1);
    auto outputBuffer = data->getFiles().at(0).buffer();

    outputBuffer->open(QIODevice::ReadOnly);
    QCOMPARE(QString(outputBuffer->readAll()), Test1OutputContents);
    outputBuffer->close();

    data->deleteLater();
    archive->deleteLater();
}

void QArchiveMemoryCompressorTests::usingPauseResume() {
    QArchive::MemoryCompressor compressor(QArchive::SevenZipFormat);

    auto array = Test2OutputContents.toLocal8Bit();

    QBuffer inputBuffer(&array);
    compressor.addFiles(QFileInfo(Test2OutputFile).fileName(), &inputBuffer);

    bool startedEmitted = false,
         pausedEmitted = false,
         resumedEmitted = false;
    QObject::connect(&compressor, &QArchive::MemoryCompressor::error,
                     this, &QArchiveMemoryCompressorTests::defaultErrorHandler);
    QObject::connect(&compressor, &QArchive::MemoryCompressor::started, [&]() {
        startedEmitted = true;
        compressor.pause();
        return;
    });
    QObject::connect(&compressor, &QArchive::MemoryCompressor::paused, [&]() {
        pausedEmitted = true;
        QTimer::singleShot(1000, &compressor, SLOT(resume()));
        return;
    });
    QObject::connect(&compressor, &QArchive::MemoryCompressor::resumed, [&]() {
        resumedEmitted = true;
        return;
    });

    QSignalSpy spyInfo(&compressor, &QArchive::MemoryCompressor::finished);
    compressor.start();

    QVERIFY(spyInfo.wait() || spyInfo.count());

    QVERIFY(startedEmitted);
    QVERIFY(pausedEmitted);
    QVERIFY(resumedEmitted);

    /// Verify Data

    QList<QVariant> output = spyInfo.takeFirst();
    QVERIFY(output.count() == 1);
    auto archive = output.at(0).value<QBuffer*>();

    QArchive::MemoryExtractor extractor((QIODevice*)archive);
    QObject::connect(&extractor, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryCompressorTests::defaultExtractorErrorHandler);

    QSignalSpy spyExtractor(&extractor, &QArchive::MemoryExtractor::finished);
    extractor.start();

    QVERIFY(spyExtractor.wait() || spyExtractor.count());

    QList<QVariant> extractorOutput = spyExtractor.takeFirst();

    QVERIFY(extractorOutput.count() == 1);
    auto data = extractorOutput.at(0).value<QArchive::MemoryExtractorOutput*>();

    QVERIFY(data->getFiles().count() >= 1);
    auto outputBuffer = data->getFiles().at(0).buffer();

    outputBuffer->open(QIODevice::ReadOnly);
    QCOMPARE(QString(outputBuffer->readAll()), Test2OutputContents);
    outputBuffer->close();

    data->deleteLater();
    archive->deleteLater();
}

void QArchiveMemoryCompressorTests::compressingMultipleFiles() {
    QArchive::MemoryCompressor compressor(QArchive::SevenZipFormat);

    /* Write the file to compress and add it. */
    auto array1 = Test3Output1Contents.toLocal8Bit();
    auto array2 = Test3Output2Contents.toLocal8Bit();

    QBuffer buffer1(&array1);
    QBuffer buffer2(&array2);

    compressor.addFiles(
        QStringList() << QFileInfo(Test3OutputFile1).fileName()
        << QFileInfo(Test3OutputFile2).fileName(),
        QVariantList() << QVariant::fromValue((QIODevice*)&buffer1)
        << QVariant::fromValue((QIODevice*)&buffer2));

    QObject::connect(&compressor, &QArchive::MemoryCompressor::error,
                     this, &QArchiveMemoryCompressorTests::defaultErrorHandler);
    QSignalSpy progressSpyInfo(&compressor, SIGNAL(progress(QString, int, int, qint64, qint64)));
    QSignalSpy spyInfo(&compressor, &QArchive::MemoryCompressor::finished);
    compressor.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());
    QCOMPARE(progressSpyInfo.count(), 4);

    QList<QVariant> output = spyInfo.takeFirst();
    QVERIFY(output.count() == 1);
    auto archive = output.at(0).value<QBuffer*>();

    QArchive::MemoryExtractor extractor((QIODevice*)archive);
    QObject::connect(&extractor, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryCompressorTests::defaultExtractorErrorHandler);

    QSignalSpy spyExtractor(&extractor, &QArchive::MemoryExtractor::finished);
    extractor.start();

    QVERIFY(spyExtractor.wait() || spyExtractor.count());

    QList<QVariant> extractorOutput = spyExtractor.takeFirst();

    QVERIFY(extractorOutput.count() == 1);
    auto data = extractorOutput.at(0).value<QArchive::MemoryExtractorOutput*>();

    QVERIFY(data->getFiles().count() >= 2);
    auto outputBuffer1 = data->getFiles().at(0).buffer();
    auto outputBuffer2 = data->getFiles().at(1).buffer();

    outputBuffer1->open(QIODevice::ReadOnly);
    outputBuffer2->open(QIODevice::ReadOnly);

    QCOMPARE(QString(outputBuffer1->readAll()), Test3Output1Contents);
    QCOMPARE(QString(outputBuffer2->readAll()), Test3Output2Contents);

    outputBuffer1->close();
    outputBuffer2->close();

    data->deleteLater();
    archive->deleteLater();
}

void QArchiveMemoryCompressorTests::encryptingZipArchive() {
    QArchive::MemoryCompressor compressor(QArchive::ZipFormat);

    /* Write the file to compress and add it. */
    auto array = Test4OutputContents.toLocal8Bit();
    QBuffer buffer(&array);
    compressor.addFiles(/*entry name=*/QFileInfo(Test4OutputFile).fileName(), (QIODevice*)&buffer);

    QObject::connect(&compressor, &QArchive::MemoryCompressor::error,
                     this, &QArchiveMemoryCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&compressor, &QArchive::MemoryCompressor::finished);

    compressor.setPassword(Test4Password);
    compressor.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    QList<QVariant> output = spyInfo.takeFirst();
    QVERIFY(output.count() == 1);
    auto archive = output.at(0).value<QBuffer*>();

    QArchive::MemoryExtractor extractor((QIODevice*)archive);
    QObject::connect(&extractor, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryCompressorTests::defaultExtractorErrorHandler);

    QSignalSpy spyExtractor(&extractor, &QArchive::MemoryExtractor::finished);

    extractor.setPassword(Test4Password);
    extractor.start();

    QVERIFY(spyExtractor.wait() || spyExtractor.count());

    QList<QVariant> extractorOutput = spyExtractor.takeFirst();

    QVERIFY(extractorOutput.count() == 1);
    auto data = extractorOutput.at(0).value<QArchive::MemoryExtractorOutput*>();

    QVERIFY(data->getFiles().count() >= 1);
    auto outputBuffer = data->getFiles().at(0).buffer();

    outputBuffer->open(QIODevice::ReadOnly);
    QCOMPARE(QString(outputBuffer->readAll()), Test4OutputContents);
    outputBuffer->close();

    data->deleteLater();
    archive->deleteLater();
}

void QArchiveMemoryCompressorTests::runningCompressorNonSingleThreaded() {
    QArchive::MemoryCompressor compressor(QArchive::GZipFormat, /*parent=*/nullptr,
                                          /*single threaded=*/false);

    /* Write the file to compress and add it. */
    auto array = Test5OutputContents.toLocal8Bit();
    QBuffer buffer(&array);
    compressor.addFiles(/*entry name=*/QFileInfo(Test5OutputFile).fileName(), (QIODevice*)&buffer);

    QObject::connect(&compressor, &QArchive::MemoryCompressor::error,
                     this, &QArchiveMemoryCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&compressor, &QArchive::MemoryCompressor::finished);
    compressor.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    QList<QVariant> output = spyInfo.takeFirst();
    QVERIFY(output.count() == 1);
    auto archive = output.at(0).value<QBuffer*>();

    QArchive::MemoryExtractor extractor((QIODevice*)archive);
    QObject::connect(&extractor, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryCompressorTests::defaultExtractorErrorHandler);

    QSignalSpy spyExtractor(&extractor, &QArchive::MemoryExtractor::finished);
    extractor.start();

    QVERIFY(spyExtractor.wait() || spyExtractor.count());

    QList<QVariant> extractorOutput = spyExtractor.takeFirst();

    QVERIFY(extractorOutput.count() == 1);
    auto data = extractorOutput.at(0).value<QArchive::MemoryExtractorOutput*>();

    QVERIFY(data->getFiles().count() >= 1);
    auto outputBuffer = data->getFiles().at(0).buffer();

    outputBuffer->open(QIODevice::ReadOnly);
    QCOMPARE(QString(outputBuffer->readAll()), Test5OutputContents);
    outputBuffer->close();

    data->deleteLater();
    archive->deleteLater();
}

void QArchiveMemoryCompressorTests::compressingTarArchiveWithoutFilters() {
    QArchive::MemoryCompressor compressor(QArchive::TarFormat);

    /* Write the file to compress and add it. */
    auto array = Test6OutputContents.toLocal8Bit();
    QBuffer buffer(&array);
    compressor.addFiles(/*entry name=*/QFileInfo(Test6OutputFile).fileName(), (QIODevice*)&buffer);

    QObject::connect(&compressor, &QArchive::MemoryCompressor::error,
                     this, &QArchiveMemoryCompressorTests::defaultErrorHandler);
    QSignalSpy spyInfo(&compressor, &QArchive::MemoryCompressor::finished);
    compressor.start();

    /*  Must emit exactly one signal. */
    QVERIFY(spyInfo.wait() || spyInfo.count());

    QList<QVariant> output = spyInfo.takeFirst();
    QVERIFY(output.count() == 1);
    auto archive = output.at(0).value<QBuffer*>();

    QArchive::MemoryExtractor extractor((QIODevice*)archive);
    QObject::connect(&extractor, &QArchive::MemoryExtractor::error,
                     this, &QArchiveMemoryCompressorTests::defaultExtractorErrorHandler);

    QSignalSpy spyExtractor(&extractor, &QArchive::MemoryExtractor::finished);
    extractor.start();

    QVERIFY(spyExtractor.wait() || spyExtractor.count());

    QList<QVariant> extractorOutput = spyExtractor.takeFirst();

    QVERIFY(extractorOutput.count() == 1);
    auto data = extractorOutput.at(0).value<QArchive::MemoryExtractorOutput*>();

    QVERIFY(data->getFiles().count() >= 1);
    auto outputBuffer = data->getFiles().at(0).buffer();

    outputBuffer->open(QIODevice::ReadOnly);
    QCOMPARE(QString(outputBuffer->readAll()), Test6OutputContents);
    outputBuffer->close();

    data->deleteLater();
    archive->deleteLater();
}

void QArchiveMemoryCompressorTests::defaultErrorHandler(short code, QString file) {
    auto scode = QString::number(code);
    scode.prepend(("error::" + file) + ":: ");
    QFAIL(QTest::toString(scode));
    return;
}

void QArchiveMemoryCompressorTests::defaultExtractorErrorHandler(short code) {
    auto scode = QString::number(code);
    scode.prepend("error:: ");
    QFAIL(QTest::toString(scode));
    return;
}
