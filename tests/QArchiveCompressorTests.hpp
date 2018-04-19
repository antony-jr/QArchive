#include <QArchive>
#include <QTest>

#define NUM_OF_TEST_CASES 5

class QArchiveCompressor : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase(void)
    {
        QDir cases(TestCasesDir);
        if(!cases.exists()) {
            QFAIL("CANNOT FIND TEST CASE DIRECTORY!");
            return;
        } else {
            for(int i = 1; i <= NUM_OF_TEST_CASES ; ++i) {
                if(!QFileInfo(getTestCaseFiles(i)).exists()) {
                    QFAIL("CANNOT FIND ALL TEST FILE(S)");
                    return;
                }
            }
        }
        QDir dir;
        dir.mkdir(TestOutputDir);
    }
    void simpleCompression(void)
    {
        QBENCHMARK {
            QArchive::Compressor(getTestCaseArchivePath(1, ".7z"), getTestCaseFiles(1))
            .setFunc([this](short code, QString eMsg)
            {
                switch(code) {
                case QArchive::DISK_OPEN_ERROR:
                    QFAIL(QTest::toString("unable to read file(s) :: " + eMsg));
                    break;
                default:
                    QFAIL(QTest::toString("unknown error. :: " + eMsg));
                    break;
                }
                return;
            })
            .start()
            .waitForFinished();
        }

        // TEST THE OUTPUT FILE.
        QVERIFY((QFileInfo(getTestCaseArchivePath(1, ".7z")).exists()) == true);  // DOES IT EXIST ?
        // -------

    }

    void usingPauseResume(void)
    {
        QArchive::Compressor e(getTestCaseArchivePath(2, ".zip"), getTestCaseFiles(1));
        e.setFunc(QArchive::STARTED, [&e]() {
            // Pause.
            QTimer::singleShot(100, &e, SLOT(pause()));
            return;
        })
        .setFunc(QArchive::PAUSED, [&e]() {
            // Verify if its paused.
            QVERIFY(e.isPaused() == true);
            QTimer::singleShot(1000, &e, SLOT(resume()));
            return;
        })
        .setFunc(QArchive::RESUMED, [&e]() {
            // Verify if its resumed.
            QVERIFY(e.isRunning() == true);
        })
        .setFunc([&e](short code, QString eMsg) {
            if(code) {
                QFAIL(QTest::toString("AN ERROR AS OCCURED :: " + eMsg));
            }
            return;
        })
        .start()
        .waitForFinished();

        // TEST THE OUTPUT FILE.
        QVERIFY((QFileInfo(getTestCaseArchivePath(2, ".zip")).exists()) == true);
        // -------
    }

    void usingCancel(void)
    {
        QArchive::Compressor e(getTestCaseArchivePath(3, ".tar.gz"), getTestCaseFiles(3));
        e.setFunc(QArchive::CANCELED, [&e]() {
            QVERIFY(e.isCanceled() == true); // AND IS CANCELED SUCCESSFULLY ?
            return;
        })
        .setFunc(QArchive::FINISHED, [&e]() {
            QFAIL("finish signal emitted even after cancel is requested!");
            return;
        })
        .setFunc([&e](short code, QString eMsg) {
            if(code) {
                QFAIL(QTest::toString("AN ERROR AS OCCURED :: " + eMsg));
            }
            return;
        })
        .start()
        .cancel()
        .waitForFinished();

        // DOES IT EXIT THE WAIT-FOR EVEN IF ITS CANCELED ?
        QVERIFY(true);

        // VERIFY IF THERE IS NO LEFT-OVERS
        QVERIFY(QFileInfo(getTestCaseArchivePath(3, ".tar.gz")).exists() == false);
    }

    void isCompressorObjectReuseable(void)
    {
        bool firstOperationSuccess = false,
             secondOperationSuccess = false;

        QArchive::Compressor CompressorObject(getTestCaseArchivePath(4, ".tar.bzip"), getTestCaseFiles(4));
        CompressorObject
        .setFunc([&](short code, QString file) {
            (void)file;
            if(code) {
                QFAIL("OPERATION ERRORED!");
                return;
            }
            return;
        })
        .setFunc(QArchive::FINISHED,[&firstOperationSuccess]() {
            QVERIFY((firstOperationSuccess = true));
            return;
        })
        .start()
        .waitForFinished()
        .setArchive(getTestCaseArchivePath(5, ".tar.bzip2"), getTestCaseFiles(5))
        .setFunc(QArchive::FINISHED,[&secondOperationSuccess]() {
            QVERIFY((secondOperationSuccess = true));
            return;
        })
        .start()
        .waitForFinished();

        QVERIFY(firstOperationSuccess == true);
        QVERIFY(secondOperationSuccess == true);
    }

    void addingNonExistingFiles(void)
    {
        QArchive::Compressor(getTestCaseArchivePath(6, ".7z"), getTestCaseFiles(100))
        .setFunc([this](short code, QString eMsg) {
            switch(code) {
            case QArchive::FILE_NOT_EXIST:
                QVERIFY(true);
                break;
            default:
                QFAIL(QTest::toString("unknown error. :: " + eMsg));
                break;
            }
            return;
        })
        .start()
        .waitForFinished();
    }

    void invalidArchivePath(void)
    {
        QArchive::Compressor("/dev/null", getTestCaseFiles(1))
        .setFunc([this](short code, QString eMsg) {
            switch(code) {
            case QArchive::ARCHIVE_WRITE_OPEN_ERROR:
                QVERIFY(true);
                break;
            default:
                QFAIL(QTest::toString("unknown error. :: " + eMsg));
                break;
            }
            return;
        })
        .start()
        .waitForFinished();
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

    QString getTestCaseArchivePath(int number, const QString &format)
    {
        QString TestCaseArchivePath = QString(TestOutputDir + QString("Test") + QString::number(number) + format);
        return TestCaseArchivePath;
    }
    QString getTestCaseFiles(int number)
    {
        return QString(TestCasesDir + QString("Test") + QString::number(number) + ".txt");
    }
// ------
};
