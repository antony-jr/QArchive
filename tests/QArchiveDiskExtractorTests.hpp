#ifndef QARCHIVE_DISK_EXTRACTOR_TESTS_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_TESTS_HPP_INCLUDED
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

class QArchiveDiskExtractorTests : public QObject,private QArchiveTestCases {
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
    void testInvalidArchivePath();
    void runningExtractorNonSingleThreaded();
    void extractArchiveFromQIODevice();
    void getInfoFromTarArchiveWithNoFilters();
    void extractTarArchiveWithNoFilters();
    void extractTarArchiveWithZSTD();
    void isExtractorObjectReuseable();
    void testProgress();
  protected slots:
    static void defaultErrorHandler(short code);
};
#endif // QARCHIVE_DISK_EXTRACTOR_TESTS_HPP_INCLUDED
