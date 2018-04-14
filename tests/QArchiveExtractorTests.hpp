#include <QArchive>
#include <QTest>

/*
 * Test Input file paths and other 
 * static informations required for the unit test.
*/
const QString TestCasesDir = QString("TestCases/");
const QString TestOutputDir = QString("TestOutput/");

// TEST 1
const QString TestCase1ArchivePath = QString(TestCasesDir + "Test1.7z");
const QString TestCase1OutputDir = QString(TestOutputDir + "Test1");
const QString Test1OutputContents = QString("TEST1SUCCESS!");
const QString Test1OutputFile = QString(TestCase1OutputDir + "/Output.txt");

// TEST 2
const QString TestCase2ArchivePath = QString(TestCasesDir + "Test2.7z");
const QString TestCase2OutputDir = QString(TestOutputDir + "Test2");
const QString Test2OutputContents = QString("TEST2SUCCESS!");
const QString Test2OutputFile = QString(TestCase2OutputDir + "/Output.txt");

// TEST 3
const QString TestCase3ArchivePath = QString(TestCasesDir + "Test3.7z");
const QString TestCase3OutputDir = QString(TestOutputDir + "Test3");
const QString Test3OutputFile1 = QString(TestCase3OutputDir + "/ThisMayBeExtracted.txt");
const QString Test3OutputFile2 = QString(TestCase3OutputDir + "/ThisShouldNotBeExtracted.txt");

// TEST 4
const QString TestCase4ArchivePath = QString(TestCasesDir + "Test4.zip");
const QString TestCase4OutputDir = QString(TestOutputDir + "Test4");
const QString Test4Password = QString("Test4");
const QString Test4OutputContents = QString("TEST4SUCCESS!");
const QString Test4OutputFile = QString(TestCase4OutputDir + "/Output.txt");

// TEST 5
const QString TestCase5ArchivePath = QString(TestCasesDir + "Test5.zip");
const QString TestCase5OutputDir = QString(TestOutputDir + "Test5");
 const QString Test5OutputContents = QString("TEST5SUCCESS!");
const QString Test5Password = QString("Test5");
const QString Test5WrongPassword = QString("!Test5");
const QString Test5OutputFile = QString(TestCase5OutputDir + "/Output.txt");

// TEST 6
const QString TestCase6ArchivePath = QString(TestCasesDir + "Test6.zip");
const QString TestCase6OutputDir = QString(TestOutputDir + "Test6");
 const QString Test6OutputContents = QString("TEST6SUCCESS!");
const QString Test6WrongPassword = QString("!Test6");
const QString Test6OutputFile = QString(TestCase6OutputDir + "/Output.txt");
// ------

class QArchiveExtractor : public QObject
{
    Q_OBJECT
private slots:
    void simpleExtraction(void)
    {
        QBENCHMARK {
            QArchive::Extractor(TestCase1ArchivePath, TestCase1OutputDir)
            .setFunc([this](short code, QString eMsg)
            {
                switch(code) {
                case QArchive::ARCHIVE_READ_ERROR:
                    QFAIL(QTest::toString("unable to read archive :: " + eMsg));
                    break;
                case QArchive::ARCHIVE_QUALITY_ERROR:
                    QFAIL(QTest::toString("bad archive! :: " + eMsg));
                    break;
                case QArchive::ARCHIVE_UNCAUGHT_ERROR:
                    QFAIL(QTest::toString("fatal error. :: " + eMsg));
                    break;
                case QArchive::DESTINATION_NOT_FOUND:
                    QFAIL(QTest::toString("Destination not found. ::  " + eMsg));
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
        QFile TestOutput(Test1OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true); // DOES IT EXIST ?

        // ARE THE CONTENTS NOT MESSED UP ?
        QVERIFY(Test1OutputContents == QString(TestOutput.readAll()));

        // -------

    }

    void usingPauseResume(void)
    {
        QArchive::Extractor e(TestCase2ArchivePath, TestCase2OutputDir);
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
            switch(code) {
            case QArchive::ARCHIVE_READ_ERROR:
                QFAIL(QTest::toString("unable to read archive :: " + eMsg));
                break;
            case QArchive::ARCHIVE_QUALITY_ERROR:
                QFAIL(QTest::toString("bad archive! :: " + eMsg));
                break;
            case QArchive::ARCHIVE_UNCAUGHT_ERROR:
                QFAIL(QTest::toString("fatal error. :: " + eMsg));
                break;
            case QArchive::DESTINATION_NOT_FOUND:
                QFAIL(QTest::toString("destination not found. ::  " + eMsg));
                break;
            default:
                QFAIL(QTest::toString("unknown error. :: " + eMsg));
                break;
            }
            return;
        })
        .start()
        .waitForFinished();

        // TEST THE OUTPUT FILE.
        QFile TestOutput(Test2OutputFile);
        QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true); // DOES IT EXIST ?

        // ARE THE CONTENTS NOT MESSED UP ?
        QVERIFY(Test2OutputContents == QString(TestOutput.readAll()));
        // -------
    }

    void usingCancel(void)
    {
        QArchive::Extractor e(TestCase3ArchivePath , "TestOutput/Test3/");
        e.setFunc(QArchive::CANCELED , [&e](){
                QVERIFY(e.isCanceled() == true); // AND IS CANCELED SUCCESSFULLY ?
                return;
        })
        .setFunc(QArchive::FINISHED , [&e](){
                QFAIL("finish signal emitted even after cancel is requested!");
                return;
        })
        .setFunc([&e](short code, QString eMsg) {
             switch(code) {
             case QArchive::ARCHIVE_READ_ERROR:
                 QFAIL(QTest::toString("unable to read archive :: " + eMsg));
                 break;
             case QArchive::ARCHIVE_QUALITY_ERROR:
                 QFAIL(QTest::toString("bad archive! :: " + eMsg));
                 break;
             case QArchive::ARCHIVE_UNCAUGHT_ERROR:
                 QFAIL(QTest::toString("fatal error. :: " + eMsg));
                 break;
             case QArchive::DESTINATION_NOT_FOUND:
                 QFAIL(QTest::toString("destination not found. ::  " + eMsg));
                 break;
             default:
                 QFAIL(QTest::toString("unknown error. :: " + eMsg));
                 break;
             }
             return;
         })
         .start()
         .cancel()
         .waitForFinished();

         // DOES IT EXIT THE WAIT-FOR EVEN IF ITS CANCELED ?
         QVERIFY(true);

         // VERIFY IF THE EXTRACTION WAS INCOMPLETE.
         QVERIFY(QFileInfo(Test3OutputFile1).exists() == true ||
                 QFileInfo(Test3OutputFile1).exists() == false);
         QVERIFY(QFileInfo(Test3OutputFile2).exists() == false);
    }

    void usingPassword(void)
    {
        QArchive::Extractor(TestCase4ArchivePath, TestCase4OutputDir)
        .setPassword(Test4Password)
        .setFunc([this](short code, QString eMsg)
             {
                 switch(code) {
                 case QArchive::ARCHIVE_READ_ERROR:
                     QFAIL(QTest::toString("password wrong :: " + eMsg));
                     break;
                 case QArchive::ARCHIVE_QUALITY_ERROR:
                     QFAIL(QTest::toString("bad archive! :: " + eMsg));
                     break;
                 case QArchive::ARCHIVE_UNCAUGHT_ERROR:
                     QFAIL(QTest::toString("fatal error. :: " + eMsg));
                     break;
                 case QArchive::DESTINATION_NOT_FOUND:
                     QFAIL(QTest::toString("Destination not found. ::  " + eMsg));
                     break;
                 default:
                     QFAIL(QTest::toString("unknown error. :: " + eMsg));
                     break;
                 }
                 return;
             })
             .start()
             .waitForFinished();

        // TEST IF THE FILE IS EXTRACTED
        QFile TestOutput(Test4OutputFile);
        QVERIFY(TestOutput.open(QIODevice::ReadOnly) == true);
        QVERIFY(Test4OutputContents == QString(TestOutput.readAll())); // ARE THE CONTENTS SAME ?
    }

    void usingWrongPasswordAndAfter3TriesGiveTheCorrectPassword()
    {
        QArchive::Extractor e(TestCase5ArchivePath , TestCase5OutputDir);
        e.setPassword(Test5WrongPassword)
        .setFunc(QArchive::PASSWORD_REQUIRED , [&e](int Tries){
            if(Tries == 3) { // CHECK IF THIS THE THIRD TRY.
                e.setPassword(Test5Password); // CORRECT PASSWORD.
                return;
            }
            e.setPassword(Test5WrongPassword); // WRONG PASSWORD.
            return;
        })
        .setFunc([&e](short code, QString eMsg)
              {
                  switch(code) {
                  case QArchive::ARCHIVE_READ_ERROR:
                      QFAIL(QTest::toString("password wrong :: " + eMsg));
                      break;
                  case QArchive::ARCHIVE_QUALITY_ERROR:
                      QFAIL(QTest::toString("bad archive! :: " + eMsg));
                      break;
                  case QArchive::ARCHIVE_UNCAUGHT_ERROR:
                      QFAIL(QTest::toString("fatal error. :: " + eMsg));
                      break;
                  case QArchive::DESTINATION_NOT_FOUND:
                      QFAIL(QTest::toString("Destination not found. ::  " + eMsg));
                      break;
                  case QArchive::ARCHIVE_WRONG_PASSWORD:
                      // PASS WRONG PASSWORD ERROR 
                      break;
                  default:
                      QFAIL(QTest::toString("unknown error. :: " + eMsg));
                      break;
                  }
                  return;
              })
              .start()
              .waitForFinished();

            // TEST THE OUTPUT FILE.
            QFile TestOutput(Test5OutputFile);
            QVERIFY((TestOutput.open(QIODevice::ReadOnly)) == true); // DOES IT EXIST ?
            // ARE THE CONTENTS NOT MESSED UP ?
            QVERIFY(Test5OutputContents == QString(TestOutput.readAll()));
    }

    void cancelingPasswordRequiredSignals(void)
    {
        bool gotArchivePasswordNotGiven = false;
        QArchive::Extractor e(TestCase6ArchivePath , TestCase6OutputDir);
        e.setPassword(Test6WrongPassword)
        .setFunc(QArchive::PASSWORD_REQUIRED , [&e](int Tries){
            (void)Tries; // UNUSED
            e.setPassword(QString()); // CANCEL PRESISTENT PASSWORD CALLBACK
            return;
        })
        .setFunc([&gotArchivePasswordNotGiven](short code, QString eMsg)
              {
                  switch(code) {
                  case QArchive::ARCHIVE_READ_ERROR:
                      QWARN(QTest::toString("archive read error :: " + eMsg));
                      break;
                  case QArchive::ARCHIVE_QUALITY_ERROR:
                      QFAIL(QTest::toString("bad archive! :: " + eMsg));
                      break;
                  case QArchive::ARCHIVE_UNCAUGHT_ERROR:
                      QFAIL(QTest::toString("fatal error. :: " + eMsg));
                      break;
                  case QArchive::DESTINATION_NOT_FOUND:
                      QFAIL(QTest::toString("Destination not found. ::  " + eMsg));
                      break;
                  case QArchive::ARCHIVE_PASSWORD_NOT_GIVEN:
                      QVERIFY(true);
                      gotArchivePasswordNotGiven |= true;
                      break;
                  case QArchive::ARCHIVE_WRONG_PASSWORD:
                      // PASS WRONG PASSWORD ERROR 
                      break;
                  default:
                      QFAIL(QTest::toString("unknown error. :: " + eMsg));
                      break;
                  }
                  return;
              })
              .start()
              .waitForFinished();

            // TEST THE OUTPUT FILE.
            QFileInfo TestOutput(Test6OutputFile);
            if(TestOutput.exists()){ // IF IT EXISTS DOES IT HAVE THE REAL CONTENT 
                                     // OR JUST A PLACE HOLDER.
                QFile TestFile(Test6OutputFile);
                TestFile.open(QIODevice::ReadOnly);
                QVERIFY(Test6OutputContents != QString(TestFile.readAll()));
            }

    }

    void testInvalidArchivePath(void)
    {
        bool gotArchiveReadError = false;
        QArchive::Extractor("THISDOESNOTEXISTS" , TestOutputDir)
        .setFunc([&gotArchiveReadError](short code , QString file){
            (void)file; // UNUSED
            if(code == QArchive::ARCHIVE_READ_ERROR){
                QVERIFY((gotArchiveReadError = true));
            }else{
                QFAIL("UNEXPECTED ERROR");
            }
        })
        .start()
        .waitForFinished();

        QVERIFY(gotArchiveReadError == true); // DID WE GOT ANY READ ERRORS ?
    }

    void testInvalidDestination(void)
    {
        bool gotDestinationNotFoundError = false;
         QArchive::Extractor(TestCase1ArchivePath , "THE PLACE THAT NEVER EXISTED")
         .setFunc([&gotDestinationNotFoundError](short code , QString file){
             (void)file; // UNUSED
             if(code == QArchive::DESTINATION_NOT_FOUND){
                 QVERIFY((gotDestinationNotFoundError = true));
             }else{
                 QFAIL("UNEXPECTED ERROR");
             }
         })
         .start()
         .waitForFinished();

         QVERIFY(gotDestinationNotFoundError == true); // DID WE GET THE RIGHT ERROR ?
    }

    void cleanupTestCase(void)
    {
        // DELETE ALL EXTRACTED FILES
        QFile::remove(Test1OutputFile); // TEST1
        QFile::remove(Test2OutputFile); // TEST2
        QFile::remove(Test3OutputFile1); // TEST3
        QFile::remove(Test3OutputFile2); // TEST3
        QFile::remove(Test4OutputFile); // TEST4
        QFile::remove(Test5OutputFile); // TEST5
        QFile::remove(Test6OutputFile); // TEST6
    }
};

QTEST_MAIN(QArchiveExtractor);
