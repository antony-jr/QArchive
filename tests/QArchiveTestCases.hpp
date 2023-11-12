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
  explicit QArchiveTestCases(QTemporaryDir* dir) {
    if (!dir || dir->path().isEmpty()) {
      return;
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

    TestCase9ArchivePath = TestCasesDir + "Test9.zip";
    TestCase9OutputDir = TestOutputDir + "Test9";
    Test9OutputFile = TestCase9OutputDir + "/新建文件.txt";
  }

  ~QArchiveTestCases() = default;

  QString TestCasesDir;
  QString TestOutputDir;
  QString TestCase1ArchivePath;
  QString TestCase1OutputDir;
  QString Test1OutputFile;
  QString TestCase2ArchivePath;
  QString TestCase2OutputDir;
  QString Test2OutputFile;
  QString TestCase3ArchivePath;
  QString TestCase3OutputDir;
  QString Test3OutputFile1;
  QString Test3OutputFile2;
  QString TestCase4ArchivePath;
  QString TestCase4OutputDir;
  QString Test4OutputFile;
  QString TestCase5ArchivePath;
  QString TestCase5OutputDir;
  QString Test5OutputFile;
  QString TestCase6ArchivePath;
  QString TestCase6OutputDir;
  QString Test6OutputFile;
  QString TestCase7ArchivePath;
  QString TestCase7OutputDir;
  QString Test7OutputFile;
  QString TestCase8ArchivePath;
  QString TestCase8OutputDir;
  QString Test8OutputFile;
  QString TestCase9ArchivePath;
  QString TestCase9OutputDir;
  QString Test9OutputFile;
  QString TemporaryFilePath;

 protected:
  const QString Test1OutputContents = "TEST1SUCCESS!";
  const QString Test2OutputContents = "TEST2SUCCESS!";
  const QString Test3Output1Contents = "TEST3SUCCESS1!";
  const QString Test3Output2Contents =
      "TEST3SUCCESS2!_SOME_MORE_CONTENT_TO_CALCULATE_PROGRESS";
  const QString Test4Password = "Test4";
  const QString Test4OutputContents = "TEST4SUCCESS!";
  const QString Test5OutputContents = "TEST5SUCCESS!";
  const QString Test6OutputContents = "TEST6SUCCESS!";
  const QString Test7OutputContents = "TEST7SUCCESS!";
  const QString Test9OutputContents = "测试9成功!";
};

#endif  // QARCHIVE_TEST_CASES_HPP_INCLUDED
