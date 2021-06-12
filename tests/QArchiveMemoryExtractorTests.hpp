#ifndef QARCHIVE_MEMORY_EXTRACTOR_TESTS_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_TESTS_HPP_INCLUDED
#ifdef BUILD_TESTS
# include <QArchive>
#else
# include <QArchive/QArchive>
#endif
#include <QTest>
#include <QTimer>
#include <QJsonObject>
#include <QSignalSpy>
#include <QArchiveTestCases.hpp>
#include <QTemporaryDir>

class QArchiveMemoryExtractorTests : public QObject,private QArchiveTestCases {
    Q_OBJECT
  public:
    QArchiveMemoryExtractorTests(QTemporaryDir *dir);
    ~QArchiveMemoryExtractorTests();
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
    void cleanupTestCase();
  protected slots:
    void defaultErrorHandler(short code);
};
#endif // QARCHIVE_MEMORY_EXTRACTOR_TESTS_HPP_INCLUDED
