/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2018, Antony jr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------------
 *  @filename 	 	: QArchive.hpp
 *  @description 	: C++ Cross-Platform helper library that Modernizes libarchive
 *  			  using Qt5. Simply extracts 7z  , Tarballs  , RAR
 *  			  and other supported formats by libarchive.
 * -----------------------------------------------------------------------------
*/
#if !defined (QARCHIVE_HPP_INCLUDED)
#define QARCHIVE_HPP_INCLUDED

#include <QtCore>
#include <QtConcurrentRun>

/*
 * Getting the libarchive headers for the
 * runtime operating system
*/
extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
}

namespace QArchive   // QArchive Namespace Start
{
/*
 * QArchive error codes
 * --------------------
*/
enum {
    NO_ARCHIVE_ERROR,
    ARCHIVE_QUALITY_ERROR,
    ARCHIVE_READ_ERROR,
    ARCHIVE_UNCAUGHT_ERROR,
    ARCHIVE_FATAL_ERROR,
    INVALID_DEST_PATH,
    DISK_OPEN_ERROR,
    DISK_READ_ERROR,
    FILE_NOT_EXIST
};


/*
 * Class Extractor <- Inherits QObject.
 * ---------------
 *
 *  Takes care of extraction of archives with the help
 *  of libarchive.
 *
 *  Constructors:
 *  	Extractor(QObject *parent = NULL)
 *
 *  	Extractor(const QString&) 	- Simply extract a single archive , any format
 *  			      	  , Most likely will be used for 7zip.
 *  	Extractor(const QStringList&)	- Extract all archives from the QStringList in
 *  				  order.
 *
 *  Methods:
 *	void addArchive(const QString&)     - Add a archive to the queue.
 *	void addArchive(const QStringList&) - Add a set of archives to the queue
 *	void removeArchive(const QString&)  - Removes a archive from the queue matching
 *					the QString.
 *
 *  Slots:
 *	void start(void)	      - starts the extractor.
 *	void stop(void)		      - stops the extractor.
 *  Signals:
 *	void finished()		        - emitted when all extraction job is done.
 *	void extracting(const QString&) - emitted with the filename beign extracted.
 *	void extracted(const QString&)  - emitted with the filename that has been extracted.
 *	void status(const QString& , const QString&) - emitted with the entry and the filename on extraction.
 *	void error(short , const QString&) - emitted when something goes wrong!
 *
*/
class Extractor  : public QObject
{
    Q_OBJECT

public:
    explicit Extractor(QObject *parent = nullptr);
    explicit Extractor(const QString& filename);
    explicit Extractor(const QStringList& filenames);
    explicit Extractor(const QString& filename, const QString& destination);
    explicit Extractor(const QStringList& filenames, const QString& destination);
    void addArchive(const QString& filename);
    void addArchive(const QStringList& filenames);
    void removeArchive(const QString& filename);
    void setDestination(const QString& destination);
    ~Extractor();

public slots:
    bool isRunning() const;
    void start(void);
    void stop(void);

signals:
    void stopped();
    void finished();
    void extracted(const QString&);
    void extracting(const QString&);
    void status(const QString&, const QString&);
    void error(short, const QString&);

private slots:
    QString cleanDestPath(const QString& input);
    int extract(const char* filename, const char* dest);
    int copy_data(struct archive *arch, struct archive *ext);
    char *concat(const char *dest, const char *src);
    void startExtraction();

private:
    bool stopExtraction = false; // stop flag!
    QMutex mutex; // thread-safe!
    QStringList queue;
    QString	dest;
    QFuture<void> *Promise = nullptr; // Promise suits this good than future!
}; // Extractor Class Ends

/*
 * Supported Archive Types for Compressor
 * --------------------------------------
*/
enum {
    NO_FORMAT,
    BZIP,
    BZIP2,
    CPIO,
    GZIP,
    RAR,
    ZIP,
    SEVEN_ZIP
};

/*
 * Class Compressor <- Inherits QObject.
 * ----------------
 *
 *  Compresses files and folder into a archive.
 *
 *  Constructors:
 *
 *	Compressor(QObject *parent = NULL)
 *
 *	Compressor(const QString& , const QStringList&) - sets an archive with the files from QStringList.
 *	Compressor(const QString& , const QString&) - sets an archive with a single file or folder.
 *  	Compressor(const QString&) - only set the archive path to be created.
 *
 *  Methods:
 *
 *	void setArchive(const QString&)  - sets the archive path to be created.
 *	void setArchiveFormat(short) - sets the archive format.
 *	void addFiles(const QString&)    - add a single file or folder to the archive.
 *	void addFiles(const QStringList&)- add a list of files and folders to the archive.
 *	void removeFiles(const QString&) - removes a file from the archive.
 *	void removeFiles(const QStringList&) - removes a list of files from the archive.
 *
 *  Slots:
 *	void start() - starts the compression.
 *	void stop()  - stops the compression.
 *
 *  Signals:
 *      void stopped() - Emitted when the process is stopped successfully.
 * 	void finished() - Emitted when all jobs are done.
 * 	void compressing(const QString&) - Emitted with the file beign compressed.
 * 	void compressed(const QString&)  - Emitted with file had been compressed.
 * 	void error(short , const QString&) - Emitted with error code refering a file.
 *
*/

class Compressor : public QObject
{
    Q_OBJECT
public:
    explicit Compressor(QObject *parent = nullptr);
    explicit Compressor(const QString& archive);
    explicit Compressor(const QString& archive, const QStringList& files);
    explicit Compressor(const QString& archive, const QString& file);
    void setArchive(const QString& archive);
    void setArchiveFormat(short type);
    void addFiles(const QString& file);
    void addFiles(const QStringList& files);
    void removeFiles(const QString& file);
    void removeFiles(const QStringList& files);
    ~Compressor();

public slots:
    bool isRunning() const;
    void start(void);
    void stop(void);

private slots:
    void checkNodes();
    void getArchiveFormat();
    void startCompression();

signals:
    void stopped();
    void finished();
    void compressing(const QString&);
    void compressed(const QString&);
    void error(short, const QString&);

private:
    bool stopCompression = false;
    QMutex mutex;
    QFuture<void> *Promise = nullptr;
    QString archivePath;
    QStringList nodes;
    short archiveFormat = NO_FORMAT; // Default
}; // Compressor Class Ends

/*
 * Class Reader <- Inherits QObject.
 * ------------
 *
 * Gets the list of files inside a archive.
 *
 * Constructors:
 * 	Reader(QObject *parent = NULL) - Only Constructs the reader.
 * 	Reader(const QString&) - Sets a single archive and Constructs the reader.
 *
 * Methods:
 *
 * 	void setArchive(const QString&) - Sets a single archive
 *	void clear()			- Clears everything stored in this class.
 *	const QStringList& listFiles() - get the files stored in this class.
 *
 * Slots:
 * 	start() - Starts the operation.
 * 	stop()  - Stops the operation.
 *
 * Signals:
 *      void stopped() - Emitted when stop() is successfull.
 *	void error(short , const QString&) - Emitted when something goes wrong.
 * 	void archiveFiles(const QString& , const QStringList&) - Emitted when we got all the files from the archive.
*/

class Reader : public QObject
{
    Q_OBJECT
public:
    explicit Reader(QObject *parent = nullptr);
    explicit Reader(const QString& archive);
    void setArchive(const QString& archive);
    const QStringList& listFiles();
    void clear();
    ~Reader();

public slots:
    bool isRunning() const;
    void start(void);
    void stop(void);

private slots:
    void startReading();

signals:
    void stopped(void);
    void archiveFiles(const QString&, const QStringList&);
    void error(short, const QString&);

private:
    bool stopReader = false;
    QMutex mutex;
    QString Archive;
    QStringList Files;
    QFuture<void> *Promise = nullptr;
}; // Class Reader Ends

} // QArchive Namespace Ends.
#endif // QARCHIVE_HPP_INCLUDED
