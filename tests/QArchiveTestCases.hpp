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

        TestCase1ArchivePath = TestCasesDir + "Test1.7z";
        TestCase1OutputDir = TestOutputDir + "Test1";
        Test1OutputFile = TestCase1OutputDir + "/Output.txt";

        TestCase2ArchivePath = TestCasesDir + "Test2.7z";
        TestCase2OutputDir = TestOutputDir + "Test2";
        Test2OutputFile = TestCase2OutputDir + "/Output.txt";

        TestCase3ArchivePath = TestCasesDir + "Test3.7z";
        TestCase3OutputDir = TestOutputDir + "Test3";
        Test3OutputFile1 = TestCase3OutputDir + "/Output3_1.txt";
        Test3OutputFile2 = TestCase3OutputDir + "/Output3_2.txt";

        TestCase4ArchivePath = TestCasesDir + "Test4.zip";
        TestCase4OutputDir = TestOutputDir + "Test4";
        Test4OutputFile = TestCase4OutputDir + "/Output.txt";

        TestCase5ArchivePath = TestCasesDir + "Test5.tar.gz";
        TestCase5OutputDir = TestOutputDir + "Test5";
        Test5OutputFile = TestCase5OutputDir + "/Output.txt";

        TestCase6ArchivePath = TestCasesDir + "Test6.tar";
        TestCase6OutputDir = TestOutputDir + "Test6";
        Test6OutputFile = TestCase6OutputDir + "/Output.txt";

        TestCase7ArchivePath = TestCasesDir + "Test7.tar.zstd";
        TestCase7OutputDir = TestOutputDir + "Test7";
        Test7OutputFile = TestCase7OutputDir + "/Output.txt";

        TestCase8ArchivePath = TestCasesDir + "Test8.zip";
        TestCase8OutputDir = TestOutputDir + "Test8";
        Test8OutputFile = TestCase8OutputDir + "/Test8SpecialEmptyFile.txt";
    }

    ~QArchiveTestCases() = default;

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
            TestCase7ArchivePath,
            TestCase7OutputDir,
            Test7OutputFile,
	    TestCase8ArchivePath,
	    TestCase8OutputDir,
	    Test8OutputFile,
            TemporaryFilePath;
  protected:
    const QString Test1OutputContents = "TEST1SUCCESS!";
    const QString Test2OutputContents = "TEST2SUCCESS!";
    const QString Test3Output1Contents = "TEST3SUCCESS1!";
    const QString Test3Output2Contents = "TEST3SUCCESS2!_SOME_MORE_CONTENT_TO_CALCULATE_PROGRESS";
    const QString Test4Password = "Test4";
    const QString Test4OutputContents = "TEST4SUCCESS!";
    const QString Test5OutputContents = "TEST5SUCCESS!";
    const QString Test6OutputContents = "TEST6SUCCESS!";
    const QString Test7OutputContents = "TEST7SUCCESS!";
};

#endif // QARCHIVE_TEST_CASES_HPP_INCLUDED
