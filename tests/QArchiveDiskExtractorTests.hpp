#include <QArchive>
#include <QTest>
#include <QTimer>
#include <QJsonObject>
#include <QSignalSpy>
#include <QArchiveTestCases.hpp>
#include <QTemporaryDir>

class QArchiveDiskExtractorTests : public QObject,private QArchiveTestCases
{
    Q_OBJECT
public:
    QArchiveDiskExtractorTests(QTemporaryDir *dir)
        : QObject(),
          QArchiveTestCases(dir)
    {
    }

    ~QArchiveDiskExtractorTests()
    {
    }
private slots:
    void initTestCase()
    {
        QDir cases(TestCasesDir);
        if(cases.exists()) {
            if(!(
                   QFileInfo(TestCase1ArchivePath).exists() &&
                   QFileInfo(TestCase2ArchivePath).exists() &&
                   QFileInfo(TestCase3ArchivePath).exists() &&
                   QFileInfo(TestCase4ArchivePath).exists() &&
                   QFileInfo(TestCase5ArchivePath).exists()
               )) {
                QFAIL("cannot find test case files.");
                return;
            }
        } else {

            QFAIL("cannnot find test cases.");
        }
    }

    void simpleExtraction()
    {
        QArchive::DiskExtractor e(TestCase1ArchivePath, TestCase1OutputDir);
        QObject::connect(&e, &QArchive::DiskExtractor::error,
                         this, &QArchiveDiskExtractorTests::defaultErrorHandler);
        QSignalSpy spyInfo(&e, SIGNAL(finished()));
        e.start();

        /*  Must emit exactly one signal. */
        QVERIFY(spyInfo.wait() || spyInfo.count());

        /* Test the output file and also the contents. */
        QFile TestOutput(Test1OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
        QVERIFY(Test1OutputContents == QString(TestOutput.readAll()));
    }

    void usingPauseResume()
    {
        QArchive::DiskExtractor e(TestCase2ArchivePath, TestCase2OutputDir);
        bool startedEmitted = false,
             pausedEmitted = false,
             resumedEmitted = false;
        QObject::connect(&e, &QArchive::DiskExtractor::error,
                         this, &QArchiveDiskExtractorTests::defaultErrorHandler);
        QObject::connect(&e, &QArchive::DiskExtractor::started, [&]() {
            startedEmitted = true;
            e.pause();
            return;
        });
        QObject::connect(&e, &QArchive::DiskExtractor::paused, [&]() {
            pausedEmitted = true;
            QTimer::singleShot(1000, &e, SLOT(resume()));
            return;
        });
        QObject::connect(&e, &QArchive::DiskExtractor::resumed, [&]() {
            resumedEmitted = true;
            return;
        });

        QSignalSpy spyInfo(&e, SIGNAL(finished()));
        e.start();

        QVERIFY(spyInfo.wait() || spyInfo.count());

        QVERIFY(startedEmitted);
        QVERIFY(pausedEmitted);
        QVERIFY(resumedEmitted);

        /* Verify output and contents. */
        QFile TestOutput(Test2OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
        QVERIFY(Test2OutputContents == QString(TestOutput.readAll()));
    }

    void usingExtractFilters()
    {
	/*
	 * Note:
	 * Only files present in the filter gets extracted.
	*/
	QArchive::DiskExtractor e(TestCase3ArchivePath, TestCase3OutputDir);
        QObject::connect(&e, &QArchive::DiskExtractor::error,
                         this, &QArchiveDiskExtractorTests::defaultErrorHandler);
        QSignalSpy spyInfo(&e, SIGNAL(finished()));

	e.addFilter(QFileInfo(Test3OutputFile2).fileName());
        e.start();

	/* Test3OutputFile1 should not exists. */
	QVERIFY(!QFileInfo::exists(Test3OutputFile1));

        /*  Must emit exactly one signal. */
        QVERIFY(spyInfo.wait() || spyInfo.count());

        /* Test the output file and also the contents. */
        QFile TestOutput(Test3OutputFile2);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
        QVERIFY(Test3Output2Contents == QString(TestOutput.readAll())); 
    }

    void usingPassword()
    {
        QArchive::DiskExtractor e(TestCase4ArchivePath, TestCase4OutputDir);
        e.setPassword(Test4Password);
        QObject::connect(&e, &QArchive::DiskExtractor::error,
                         this, &QArchiveDiskExtractorTests::defaultErrorHandler);

        QSignalSpy spyInfo(&e, SIGNAL(finished()));
        e.start();

        /*  Must emit exactly one signal. */
        QVERIFY(spyInfo.wait() || spyInfo.count());

        QFile TestOutput(Test4OutputFile);
        QVERIFY(TestOutput.open(QIODevice::ReadOnly) == true);
        QVERIFY(Test4OutputContents == QString(TestOutput.readAll()));
    }

    void informationExtraction()
    {
        QArchive::DiskExtractor e(TestCase5ArchivePath);
        QObject::connect(&e, &QArchive::DiskExtractor::error,
                         this, &QArchiveDiskExtractorTests::defaultErrorHandler);

        QSignalSpy spyInfo(&e, SIGNAL(info(QJsonObject)));
        e.getInfo();
        QVERIFY(spyInfo.wait() || spyInfo.count());
    }

    void testInvalidArchivePath()
    {
        QArchive::DiskExtractor e("THISDOESNOTEXISTS", TestOutputDir);
        QSignalSpy spyInfo(&e, SIGNAL(error(short, QString)));
        e.start();
        QVERIFY(spyInfo.wait() || spyInfo.count());
    }

    void isExtractorObjectReuseable()
    {
        QArchive::DiskExtractor e(TestCase5ArchivePath, TestCase5OutputDir);
        QObject::connect(&e, &QArchive::DiskExtractor::error,
                         this, &QArchiveDiskExtractorTests::defaultErrorHandler);

        QFile TestOutput;
        QSignalSpy spyInfo(&e, SIGNAL(finished()));
        e.start();

        QVERIFY(spyInfo.wait() || spyInfo.count() == 1);
        TestOutput.setFileName(Test5OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
        QVERIFY(Test5OutputContents == QString(TestOutput.readAll()));
        TestOutput.close();

        e.setArchive(TestCase4ArchivePath, TestCase4OutputDir);
        e.setPassword(Test4Password);
	e.start();

        QVERIFY(spyInfo.wait() || spyInfo.count() == 2);
        TestOutput.setFileName(Test4OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
        QVERIFY(Test4OutputContents == QString(TestOutput.readAll()));
    }

    void cleanupTestCase()
    {
        QDir dir(TestOutputDir);
        dir.removeRecursively();
    }
protected slots:
    void defaultErrorHandler(short code, QString archive)
    {
        auto scode = QString::number(code);
        scode.prepend(("error::" + archive) + ":: ");
        QFAIL(QTest::toString(scode));
        return;
    }
};
