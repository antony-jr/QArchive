#ifndef QARCHIVE_DISK_COMPRESSOR_TESTS_HPP_INCLUDED
#define QARCHIVE_DISK_COMPRESSOR_TESTS_HPP_INCLUDED
#ifdef BUILD_TESTS
#include <QArchive>
#else
#include <QArchive/QArchive>
#endif
#include <QArchiveTestCases.hpp>
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>

class QArchiveDiskCompressorTests : public QObject, private QArchiveTestCases {
  using QArchiveTestCases::QArchiveTestCases;
  Q_OBJECT
 private slots:
  void initTestCase();

  void simpleCompression();
  void usingPauseResume();
  void compressingMultipleFiles();
  void encryptingZipArchive();
  void runningCompressorNonSingleThreaded();
  void compressingTarArchiveWithoutFilters();
  void compressingTarArchiveWithZSTD();
  void compressEmptyFiles();

 protected slots:
  static void defaultErrorHandler(short code, const QString& file);
};
#endif  // QARCHIVE_DISK_COMPRESSOR_TESTS_HPP_INCLUDED
