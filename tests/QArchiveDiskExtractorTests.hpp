#ifndef QARCHIVE_DISK_EXTRACTOR_TESTS_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_TESTS_HPP_INCLUDED
#include <QArchive/QArchive>
#include <QTest>
#include <QTimer>
#include <QJsonObject>
#include <QSignalSpy>
#include <QArchiveTestCases.hpp>
#include <QTemporaryDir>

class QArchiveDiskExtractorTests : public QObject,private QArchiveTestCases {
    Q_OBJECT
  public:
    QArchiveDiskExtractorTests(QTemporaryDir *dir);
    ~QArchiveDiskExtractorTests();
  private slots:
    void initTestCase();
    void simpleExtraction();
    void usingPauseResume();
    void usingExtractFilters();
    void usingPassword();
    void informationExtraction();
    void testInvalidArchivePath();
    void runningExtractorNonSingleThreaded();
    void extractArchiveFromQIODevice();
    void getInfoFromTarArchiveWithNoFilters();
    void extractTarArchiveWithNoFilters();
    void isExtractorObjectReuseable();
    void testProgress();
    void canDeleteSourceAfterUse();
    void cleanupTestCase();
  protected slots:
    void defaultErrorHandler(short code);
};
#endif // QARCHIVE_DISK_EXTRACTOR_TESTS_HPP_INCLUDED
