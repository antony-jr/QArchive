#ifndef QARCHIVE_MEMORY_COMPRESSOR_TESTS_HPP_INCLUDED
#define QARCHIVE_MEMORY_COMPRESSOR_TESTS_HPP_INCLUDED
#ifdef BUILD_TESTS
#include <QArchive>
#else
#include <QArchive/QArchive>
#endif
#include <QFile>
#include <QFileInfo>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTemporaryDir>
#include <QTest>
#include <QTimer>

#include "QArchiveTestCases.hpp"

class QArchiveMemoryCompressorTests : public QObject,
                                      private QArchiveTestCases {
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
  static void defaultExtractorErrorHandler(short code);
};
#endif  // QARCHIVE_MEMORY_COMPRESSOR_TESTS_HPP_INCLUDED
