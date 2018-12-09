#include <QArchive>
#include <QTest>
#include <QTimer>
#include <QSignalSpy>


class QArchiveDiskExtractor : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(void)
    {
        QDir cases(TestCasesDir);
        if(cases.exists()) {
            if(!(
                   QFileInfo(TestCase1ArchivePath).exists() ||
                   QFileInfo(TestCase2ArchivePath).exists() ||
                   QFileInfo(TestCase3ArchivePath).exists() ||
                   QFileInfo(TestCase4ArchivePath).exists() ||
                   QFileInfo(TestCase5ArchivePath).exists() ||
                   QFileInfo(TestCase6ArchivePath).exists() ||
                   QFileInfo(TestCase7ArchivePath).exists()
               )) {
                QFAIL("cannot find test case files.");
                return;
            }
        } else {

            QFAIL("cannnot find test cases.");
        }
        QDir dir;
        dir.mkpath(TestCase1OutputDir);
        dir.mkpath(TestCase2OutputDir);
        dir.mkpath(TestCase3OutputDir);
        dir.mkpath(TestCase4OutputDir);
        dir.mkpath(TestCase5OutputDir);
        dir.mkpath(TestCase6OutputDir);
        dir.mkpath(TestCase7OutputDir);
    }
    void simpleExtraction(void)
    {
        QArchive::DiskExtractor e(TestCase1ArchivePath, TestCase1OutputDir);
	QObject::connect(&e , &QArchive::DiskExtractor::error , [&](short code)
         {
	        auto scode = QString::number(code);
		scode.prepend("error :: ");
                QFAIL(QTest::toString(scode));
                return;
            });
	    QSignalSpy spyInfo(&e, SIGNAL(finished()));
            e.start();
	    
	    /*  Must emit exactly one signal. */
            QVERIFY(spyInfo.wait() || spyInfo.count());

        /* Test the output file and also the contents. */
	QFile TestOutput(Test1OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
	QVERIFY(Test1OutputContents == QString(TestOutput.readAll()));
    }

    void usingPauseResume(void)
    {
        QArchive::DiskExtractor e(TestCase2ArchivePath, TestCase2OutputDir);
	bool startedEmitted = false,
	     pausedEmitted = false,
	     resumedEmitted = false;
	QObject::connect(&e , &QArchive::DiskExtractor::started , [&]() {
            startedEmitted = true;
	    e.pause();
	    return;
        });
        QObject::connect(&e , &QArchive::DiskExtractor::paused , [&]() {
	    pausedEmitted = true;
	    QTimer::singleShot(1000, &e, SLOT(resume()));
            return;
        });
	QObject::connect(&e , &QArchive::DiskExtractor::resumed , [&]() {
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

    void usingPassword(void)
    {
        QArchive::DiskExtractor e(TestCase4ArchivePath, TestCase4OutputDir);
        e.setPassword(Test4Password);
        QObject::connect(&e , &QArchive::DiskExtractor::error , [&](short code)
	{
	    auto scode = QString::number(code);
	    scode.prepend("error :: ");
	    QFAIL(QTest::toString(scode));
            return;
        });
	
	QSignalSpy spyInfo(&e, SIGNAL(finished()));
        e.start();
	    
	/*  Must emit exactly one signal. */
        QVERIFY(spyInfo.wait() || spyInfo.count());
	
	QFile TestOutput(Test4OutputFile);
        QVERIFY(TestOutput.open(QIODevice::ReadOnly) == true);
        QVERIFY(Test4OutputContents == QString(TestOutput.readAll()));
    }

    void testInvalidArchivePath(void)
    {
        QArchive::DiskExtractor e("THISDOESNOTEXISTS", TestOutputDir);
	QSignalSpy spyInfo(&e, SIGNAL(error(short)));
        e.start();
        QVERIFY(spyInfo.wait() || spyInfo.count());
    }

    void isExtractorObjectReuseable(void)
    {
        QArchive::DiskExtractor e(TestCase5ArchivePath, TestCase5OutputDir);
	QFile TestOutput;
	QSignalSpy spyInfo(&e, SIGNAL(finished()));
	e.setPassword(Test5Password);
        e.start();

	QVERIFY(spyInfo.wait() || spyInfo.count() == 1);
	TestOutput.setFileName(Test5OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
        QVERIFY(Test5OutputContents == QString(TestOutput.readAll()));
	TestOutput.close();

        e.setArchive(TestCase7ArchivePath, TestCase7OutputDir);
	e.start();

	QVERIFY(spyInfo.wait() || spyInfo.count() == 2);
	TestOutput.setFileName(Test7OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true);
        QVERIFY(Test7OutputContents == QString(TestOutput.readAll()));
    }
    
    void cleanupTestCase(void)
    {
	 QDir dir(TestOutputDir);
         dir.removeRecursively();
    }

private:
    /*
     * Test Input file paths and other
     * static informations required for the unit test.
    */
    const QString TestCasesDir = QString("TestCases/");
    const QString TestOutputDir = QString("TestOutput/");

    const QString TestCase1ArchivePath = QString(TestCasesDir + "Test1.7z");
    const QString TestCase1OutputDir = QString(TestOutputDir + "Test1");
    const QString Test1OutputContents = QString("TEST1SUCCESS!");
    const QString Test1OutputFile = QString(TestCase1OutputDir + "/Output.txt");

    const QString TestCase2ArchivePath = QString(TestCasesDir + "Test2.7z");
    const QString TestCase2OutputDir = QString(TestOutputDir + "Test2");
    const QString Test2OutputContents = QString("TEST2SUCCESS!");
    const QString Test2OutputFile = QString(TestCase2OutputDir + "/Output.txt");

    const QString TestCase3ArchivePath = QString(TestCasesDir + "Test3.7z");
    const QString TestCase3OutputDir = QString(TestOutputDir + "Test3");
    const QString Test3OutputFile1 = QString(TestCase3OutputDir + "/ThisMayBeExtracted.txt");
    const QString Test3OutputFile2 = QString(TestCase3OutputDir + "/ThisShouldNotBeExtracted.txt");

    const QString TestCase4ArchivePath = QString(TestCasesDir + "Test4.zip");
    const QString TestCase4OutputDir = QString(TestOutputDir + "Test4");
    const QString Test4Password = QString("Test4");
    const QString Test4OutputContents = QString("TEST4SUCCESS!");
    const QString Test4OutputFile = QString(TestCase4OutputDir + "/Output.txt");

    const QString TestCase5ArchivePath = QString(TestCasesDir + "Test5.zip");
    const QString TestCase5OutputDir = QString(TestOutputDir + "Test5");
    const QString Test5OutputContents = QString("TEST5SUCCESS!");
    const QString Test5Password = QString("Test5");
    const QString Test5WrongPassword = QString("!Test5");
    const QString Test5OutputFile = QString(TestCase5OutputDir + "/Output.txt");

    const QString TestCase6ArchivePath = QString(TestCasesDir + "Test6.zip");
    const QString TestCase6OutputDir = QString(TestOutputDir + "Test6");
    const QString Test6OutputContents = QString("TEST6SUCCESS!");
    const QString Test6WrongPassword = QString("!Test6");
    const QString Test6OutputFile = QString(TestCase6OutputDir + "/Output.txt");

    const QString TestCase7ArchivePath = QString(TestCasesDir + "Test7.tar.gz");
    const QString TestCase7OutputDir = QString(TestOutputDir + "Test7");
    const QString Test7OutputContents = QString("TEST7SUCCESS");
    const QString Test7OutputFile = QString(TestCase7OutputDir + "/Output.txt");
};
