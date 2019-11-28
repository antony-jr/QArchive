#ifndef QARCHIVE_TEST_CASES_HPP_INCLUDED
#define QARCHIVE_TEST_CASES_HPP_INCLUDED
#include <QString>
#include <QTemporaryDir>

/*
 * Abstract class to hold the test case information.
*/
class QArchiveTestCases {
  public:
    /*
      * Test Input file paths and other
      * static informations required for the unit test.
     */
    QArchiveTestCases(QTemporaryDir *dir) {
        if(!dir) {
            return;
        } else {
            if((dir->path()).isEmpty()) {
                return;
            }
        }
        TemporaryFilePath = dir->path() + "/TemporaryFile.txt";
        TestCasesDir = dir->path() + "/";
        TestOutputDir = dir->path() + "/";

        TestCase1ArchivePath = QString(TestCasesDir + "Test1.7z");
        TestCase1OutputDir = QString(TestOutputDir + "Test1");
        Test1OutputFile = QString(TestCase1OutputDir + "/Output.txt");

        TestCase2ArchivePath = QString(TestCasesDir + "Test2.7z");
        TestCase2OutputDir = QString(TestOutputDir + "Test2");
        Test2OutputFile = QString(TestCase2OutputDir + "/Output.txt");

        TestCase3ArchivePath = QString(TestCasesDir + "Test3.7z");
        TestCase3OutputDir = QString(TestOutputDir + "Test3");
        Test3OutputFile1 = QString(TestCase3OutputDir + "/Output3_1.txt");
        Test3OutputFile2 = QString(TestCase3OutputDir + "/Output3_2.txt");

        TestCase4ArchivePath = QString(TestCasesDir + "Test4.zip");
        TestCase4OutputDir = QString(TestOutputDir + "Test4");
        Test4OutputFile = QString(TestCase4OutputDir + "/Output.txt");

        TestCase5ArchivePath = QString(TestCasesDir + "Test5.tar.gz");
        TestCase5OutputDir = QString(TestOutputDir + "Test5");
        Test5OutputFile = QString(TestCase5OutputDir + "/Output.txt");

        TestCase6ArchivePath = QString(TestCasesDir + "Test6.tar");
        TestCase6OutputDir = QString(TestOutputDir + "Test6");
        Test6OutputFile = QString(TestCase6OutputDir + "/Output.txt");
    }

    ~QArchiveTestCases() {
    }

    QString TestCasesDir,
            TestOutputDir,
            TestCase1ArchivePath,
            TestCase1OutputDir,
            Test1OutputFile,
            TestCase2ArchivePath,
            TestCase2OutputDir,
            Test2OutputFile,
            TestCase3ArchivePath,
            TestCase3OutputDir,
            Test3OutputFile1,
            Test3OutputFile2,
            TestCase4ArchivePath,
            TestCase4OutputDir,
            Test4OutputFile,
            TestCase5ArchivePath,
            TestCase5OutputDir,
            Test5OutputFile,
            TestCase6ArchivePath,
            TestCase6OutputDir,
            Test6OutputFile,
            TemporaryFilePath;
    const QString Test1OutputContents = QString("TEST1SUCCESS!");
    const QString Test2OutputContents = QString("TEST2SUCCESS!");
    const QString Test3Output1Contents = QString("TEST3SUCCESS1!");
    const QString Test3Output2Contents = QString("TEST3SUCCESS2!_SOME_MORE_CONTENT_TO_CALCULATE_PROGRESS");
    const QString Test4Password = QString("Test4");
    const QString Test4OutputContents = QString("TEST4SUCCESS!");
    const QString Test5OutputContents = QString("TEST5SUCCESS!");
    const QString Test6OutputContents = QString("TEST6SUCCESS!");
};

#endif // QARCHIVE_TEST_CASES_HPP_INCLUDED
