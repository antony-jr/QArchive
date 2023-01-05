#ifndef QARCHIVE_MEMORY_EXTRACTOR_TESTS_HPP_INCLUDED
#define QARCHIVE_MEMORY_EXTRACTOR_TESTS_HPP_INCLUDED
#ifdef BUILD_TESTS
# include <QArchive>
#else
# include <QArchive/QArchive>
#endif
#include <QArchiveTestCases.hpp>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>

class QArchiveMemoryExtractorTests : public QObject,private QArchiveTestCases {
  using QArchiveTestCases::QArchiveTestCases;
    Q_OBJECT
  private slots:
    void initTestCase();
    void simpleExtraction();
    void usingPauseResume();
    void usingExtractFilters();
    void usingPassword();
    void informationExtraction();
    void startAfterInformationExtraction();
    static void testInvalidArchivePath();
    void runningExtractorNonSingleThreaded();
    void extractArchiveFromQIODevice();
    void getInfoFromTarArchiveWithNoFilters();
    void extractTarArchiveWithNoFilters();
    void isExtractorObjectReuseable();
    void testProgress();
    void cleanupTestCase();
  protected slots:
    static void defaultErrorHandler(short code);
};
#endif // QARCHIVE_MEMORY_EXTRACTOR_TESTS_HPP_INCLUDED
