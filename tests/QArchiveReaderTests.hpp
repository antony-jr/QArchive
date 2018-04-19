#include <QArchive>
#include <QTest>



class QArchiveReader : public QObject
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
                   QFileInfo(TestCase7ArchivePath).exists()
               )) {
                QFAIL("CANNOT FIND TEST CASE ARCHIVES");
                return;
            }
        } else {

            QFAIL("CANNOT FIND TEST CASES!");
        }
    }
    void simpleReading(void)
    {
        QBENCHMARK {
            QArchive::Reader(TestCase1ArchivePath)
            .setFunc([&](QJsonObject files)
            {
                QVERIFY(!files["Output.txt"].toObject().isEmpty());
            })
            .setFunc([&](short code, QString eMsg)
            {
                if(code) {
                    QFAIL(QTest::toString("OPERATION ERRORED :: " + eMsg));
                }
                return;
            })
            .start()
            .waitForFinished();
        }

        // -------

    }

    void usingPauseResume(void)
    {
        QArchive::Reader r(TestCase2ArchivePath);
        r.setFunc(QArchive::STARTED, [&]() {
            // Pause.
            QTimer::singleShot(100, &r, SLOT(pause()));
            return;
        })
        .setFunc(QArchive::PAUSED, [&]() {
            // Verify if its paused.
            QVERIFY(r.isPaused() == true);
            QTimer::singleShot(1000, &r, SLOT(resume()));
            return;
        })
        .setFunc(QArchive::RESUMED, [&]() {
            // Verify if its resumed.
            QVERIFY(r.isRunning() == true);
        })
        .setFunc([&](QJsonObject files) {
            QVERIFY(!files["Output.txt"].toObject().isEmpty());
        })
        .setFunc([&](short code, QString eMsg) {
            if(code) {
                QFAIL(QTest::toString("OPERATION ERRORED :: " + eMsg));
            }
            return;
        })
        .start()
        .waitForFinished();

        // -------
    }

    void usingCancel(void)
    {
        QArchive::Reader r(TestCase3ArchivePath);
        r.setFunc(QArchive::CANCELED, [&]() {
            QVERIFY(r.isCanceled() == true); // AND IS CANCELED SUCCESSFULLY ?
            return;
        })
        .setFunc(QArchive::FINISHED, [&]() {
            QFAIL("finish signal emitted even after cancel is requested!");
            return;
        })
        .setFunc([&](short code, QString eMsg) {
            if(code) {
                QFAIL(QTest::toString("OPERATION ERRORED :: " + eMsg));
            }
            return;
        })
        .start()
        .cancel()
        .waitForFinished();

        // DOES IT EXIT THE WAIT-FOR EVEN IF ITS CANCELED ?
        QVERIFY(true);
    }

    void usingPassword(void)
    {
        QArchive::Reader(TestCase4ArchivePath)
        .setPassword(Test4Password)
        .setFunc([&](short code, QString eMsg) {
            if(code) {
                QFAIL(QTest::toString("OPERATION ERRORD :: " + eMsg));
            }
            return;
        })
        .setFunc([&](QJsonObject files) {
            QVERIFY(!files["Output.txt"].toObject().isEmpty());
        })
        .start()
        .waitForFinished();
    }

    void usingWrongPasswordAndAfter3TriesGiveTheCorrectPassword()
    {
        QArchive::Reader r(TestCase5ArchivePath);
        r
        .setAskPassword(true)
        .setPassword(Test5WrongPassword)
        .setFunc([&](int Tries) {
            if(Tries == 3) { // CHECK IF THIS THE THIRD TRY.
                r.setPassword(Test5Password); // CORRECT PASSWORD.
                return;
            }
            r.setPassword(Test5WrongPassword); // WRONG PASSWORD.
            return;
        })
        .setFunc([&](QJsonObject files) {
            QVERIFY(!files["Output.txt"].toObject().isEmpty());
        })
        .setFunc([&](short code, QString eMsg) {
            if(code) {
                QFAIL(QTest::toString("unknown error. :: " + eMsg));
            }
            return;
        })
        .start()
        .waitForFinished();
    }

    void testInvalidArchivePath(void)
    {
        bool gotArchiveReadError = false;
        QArchive::Reader("THISDOESNOTEXISTS")
        .setFunc([&](short code, QString file) {
            (void)file; // UNUSED
            if(code == QArchive::ARCHIVE_READ_ERROR) {
                QVERIFY((gotArchiveReadError = true));
            } else {
                QFAIL("UNEXPECTED ERROR");
            }
        })
        .start()
        .waitForFinished();

        QVERIFY(gotArchiveReadError == true); // DID WE GOT ANY READ ERRORS ?
    }


    void isReaderObjectReuseable(void)
    {
        bool firstOperationSuccess = false,
             secondOperationSuccess = false;

        QArchive::Reader(TestCase1ArchivePath)
        .setFunc([&](short code, QString file) {
            (void)file;
            if(code) {
                QFAIL(QTest::toString("OPERATION ERRORED :: " + file));
                return;
            }
            return;
        })
        .setFunc(QArchive::FINISHED,[&]() {
            QVERIFY((firstOperationSuccess = true));
            return;
        })
        .start()
        .waitForFinished()
        .setArchive(TestCase2ArchivePath)
        .setFunc(QArchive::FINISHED,[&]() {
            QVERIFY((secondOperationSuccess = true));
            return;
        })
        .start()
        .waitForFinished();

        QVERIFY(firstOperationSuccess == true);
        QVERIFY(secondOperationSuccess == true);
    }

    void supportForTar(void)
    {
        QArchive::Reader(TestCase7ArchivePath)
        .setFunc([&](short code, QString file) {
            if(code) {
                QFAIL(QTest::toString("OPERATION ERRORED :: " + file));
                return;
            }
            return;
        })
        .setFunc([&](QJsonObject files) {
            QVERIFY(!files["Output.txt"].toObject().isEmpty());
        })
        .start()
        .waitForFinished();
    }

private:
    /*
     * Test Input file paths and other
     * static informations required for the unit test.
    */
    const QString TestCasesDir = QString("TestCases/");
    const QString TestOutputDir = QString("TestOutput/");

// TEST 1
    const QString TestCase1ArchivePath = QString(TestCasesDir + "Test1.7z");
// TEST 2
    const QString TestCase2ArchivePath = QString(TestCasesDir + "Test2.7z");
// TEST 3
    const QString TestCase3ArchivePath = QString(TestCasesDir + "Test3.7z");
// TEST 4
    const QString TestCase4ArchivePath = QString(TestCasesDir + "Test4.zip");
    const QString Test4Password = QString("Test4");
// TEST 5
    const QString TestCase5ArchivePath = QString(TestCasesDir + "Test5.zip");
    const QString Test5Password = QString("Test5");
    const QString Test5WrongPassword = QString("!Test5");

// TEST FOR SUPPORT TAR.
    const QString TestCase7ArchivePath = QString(TestCasesDir + "Test7.tar.gz");
// ------
};
