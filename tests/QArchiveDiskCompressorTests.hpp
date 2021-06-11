#ifndef QARCHIVE_DISK_COMPRESSOR_TESTS_HPP_INCLUDED
#define QARCHIVE_DISK_COMPRESSOR_TESTS_HPP_INCLUDED
#ifdef BUILD_TESTS
# include <QArchive>
#else
# include <QArchive/QArchive>
#endif
#include <QTest>
#include <QFileInfo>
#include <QTimer>
#include <QFile>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QArchiveTestCases.hpp>

class QArchiveDiskCompressorTests : public QObject,private QArchiveTestCases {
    Q_OBJECT
  public:
    QArchiveDiskCompressorTests(QTemporaryDir *dir);
    ~QArchiveDiskCompressorTests();
  private slots:
    void initTestCase();

    void simpleCompression();
    void usingPauseResume();
    void compressingMultipleFiles();
    void encryptingZipArchive();
    void runningCompressorNonSingleThreaded();
    void compressingTarArchiveWithoutFilters();
  protected slots:
    void defaultErrorHandler(short code, QString file);
};
#endif // QARCHIVE_DISK_COMPRESSOR_TESTS_HPP_INCLUDED
