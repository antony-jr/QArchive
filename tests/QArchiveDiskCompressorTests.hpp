#include <QArchive>
#include <QTest>
#include <QFileInfo>
#include <QTimer>
#include <QFile>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QArchiveTestCases.hpp>

class QArchiveDiskCompressorTests : public QObject,private QArchiveTestCases
{
    Q_OBJECT
public:
    QArchiveDiskCompressorTests(QTemporaryDir *dir)
        : QObject(),
          QArchiveTestCases(dir)
    {
    }

    ~QArchiveDiskCompressorTests()
    {
    }

private slots:
    void initTestCase(void)
    {
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
        dir.mkpath(TestCase7OutputDir);

        return;
    }

    void simpleCompression(void)
    {
        QArchive::DiskCompressor e(TestCase1ArchivePath);

        /* Write the file to compress and add it. */
        QFile TestOutput(Test1OutputFile);
        QVERIFY((TestOutput.open(QIODevice::WriteOnly)) == true);
        TestOutput.write(Test1OutputContents.toLatin1());
        TestOutput.close();

        e.addFiles(/*entry name=*/"Test1.txt", Test1OutputFile);

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
protected slots:
    void defaultErrorHandler(short code, QString file)
    {
        auto scode = QString::number(code);
        scode.prepend(("error::" + file) + ":: ");
        QFAIL(QTest::toString(scode));
        return;
    }
};
