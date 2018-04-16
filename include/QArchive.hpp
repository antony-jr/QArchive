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
 *  			      using Qt5. Simply extracts 7z  , Tarballs  , RAR
 *  			      and other supported formats by libarchive.
 * -----------------------------------------------------------------------------
*/
#if !defined (QARCHIVE_HPP_INCLUDED)
#define QARCHIVE_HPP_INCLUDED

#include <QtCore>
#include <QtConcurrent>
#include <functional>

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}

/*
 * To fix build errors on vs.
 * --------------------------
 *  Fixed by https://github.com/hcaihao
*/
#if defined(_MSC_VER)
#include <io.h>
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif
// ------

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
    ARCHIVE_WRONG_PASSWORD,
    ARCHIVE_PASSWORD_NOT_GIVEN,
    ARCHIVE_WRITE_OPEN_ERROR,
    DISK_OPEN_ERROR,
    DISK_READ_ERROR,
    DESTINATION_NOT_FOUND,
    FILE_NOT_EXIST,
    INVALID_DEST_PATH,
    NOT_ENOUGH_MEMORY,
    FILE_OPEN_FAILED
};
// ---

/*
* Signal Codes for
* setFunc.
*/
enum {
    CANCELED,
    COMPRESSED,
    COMPRESSING,
    EXTRACTED,
    EXTRACTING,
    FINISHED,
    PASSWORD_REQUIRED,
    PROGRESS,
    RESUMED,
    STARTED,
    PAUSED
};

// ---

/*
 * Class UNBlock <- Inherits QObject.
 * -------------
 *
 *  This is a advanced for-loop with C++11 features
 *  backed up by Qt5 Framework.
*/
class UNBlock : public QObject
{
    Q_OBJECT
public:
    explicit UNBlock(QObject *parent = nullptr);
    explicit UNBlock(
        std::function<int(void)> initializer,
        std::function<int(void)> condition,
        std::function<void(void)> expression,
        std::function<int(void)> block,
        std::function<void(int)> deinitializer,
        int endpoint,
        int TIterations = 0);

    UNBlock &setInitializer(std::function<int(void)>);
    UNBlock &setCondition(std::function<int(void)>);
    UNBlock &setExpression(std::function<void(void)>);
    UNBlock &setCodeBlock(std::function<int(void)>);
    UNBlock &setDeInitializer(std::function<void(int)>);
    UNBlock &setEndpoint(int);
    UNBlock &setTotalIterations(int);

    ~UNBlock();
public Q_SLOTS:
    UNBlock &waitForFinished(void);
    UNBlock &start(void);
    UNBlock &cancel(void);
    UNBlock &pause(void);
    UNBlock &resume(void);

    bool isRunning(void) const;
    bool isCanceled(void) const;
    bool isPaused(void) const;
    bool isStarted(void) const;
private Q_SLOTS:
    void loop(void);
    void setStarted(bool);
    void setPaused(bool);
    void setCanceled(bool);
    void setCancelRequested(bool);
    void setPauseRequested(bool);
Q_SIGNALS:
    void started(void);
    void finished(void);
    void paused(void);
    void resumed(void);
    void doResume(void);
    void canceled(void);
    void progress(int);

private:
    bool _bStarted = false,
         _bPaused = false,
         _bCanceled = false,
         _bIsCancelRequested = false,
         _bIsPauseRequested = false;

    std::function<int(void)> initializer;
    std::function<int(void)> condition;
    std::function<void(void)> expression;
    std::function<int(void)> codeBlock;
    std::function<void(int)> deinitializer;
    int endpoint;
    int TIterations = 0;

    QMutex Mutex;
    QFuture<void> *Future = nullptr;
};

/*
 * -------
*/

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
 *	    void addArchive(const QString&)     - Add a archive to the queue.
 *	    void addArchive(const QStringList&) - Add a set of archives to the queue
 *	    void removeArchive(const QString&)  - Removes a archive from the queue matching
 *					the QString.
 *
 *  Slots:
 *	    void start(void)	      - starts the extractor.
 *	    void stop(void)		      - stops the extractor.
 *  Signals:
 *	    void finished()		        - emitted when all extraction job is done.
 *	    void extracting(const QString&) - emitted with the filename beign extracted.
 *	    void extracted(const QString&)  - emitted with the filename that has been extracted.
 *	    void status(const QString& , const QString&) - emitted with the entry and the filename on extraction.
 *	    void error(short , const QString&) - emitted when something goes wrong!
 *
*/

class Extractor  : public QObject
{
    Q_OBJECT
public:
    explicit Extractor(QObject *parent = nullptr);
    explicit Extractor(const QString&);
    explicit Extractor(const QString&, const QString&);
    Extractor &setArchive(const QString&);
    Extractor &setArchive(const QString&, const QString&);
    Extractor &setPassword(const QString&);
    Extractor &setAskPassword(bool);
    Extractor &setBlocksize(int);
    Extractor &onlyExtract(const QString&);
    Extractor &onlyExtract(const QStringList&);
    ~Extractor();

public Q_SLOTS:
    bool isRunning() const;
    bool isCanceled() const;
    bool isPaused() const;
    bool isStarted() const;

    Extractor &waitForFinished(void);
    Extractor &start(void);
    Extractor &pause(void);
    Extractor &resume(void);
    Extractor &cancel(void);

    Extractor &setFunc(short, std::function<void(void)>);
    Extractor &setFunc(short, std::function<void(QString)>); // extracting and extracted.
    Extractor &setFunc(short, std::function<void(int)>);  // progress bar and password required.
    Extractor &setFunc(std::function<void(short,QString)>); // error.
Q_SIGNALS:
    void started(void);
    void finished(void);
    void paused(void);
    void resumed(void);
    void canceled(void);
    void progress(int);
    void passwordRequired(int);
    void submitPassword(void);
    void extracted(const QString&);
    void extracting(const QString&);
    void error(short, const QString&);

private Q_SLOTS:
    // The actual extractor.
    int init(void); // Allocate and Open Archive.
    int condition(); // evaluates the condition. ( Checks if EOF ).
    int loopContent(void); // This is the loop body. ( Writes to Disk ).

    // Counts the total number of files in the archive.
    // Warning: This function is sync.
    int totalFileCount(void);
    // ---

    // utils
    void clear(void);
    QString cleanDestPath(const QString& input);
    char *concat(const char *dest, const char *src);
private:
    int ret = 0;
    QSharedPointer<struct archive> archive;
    QSharedPointer<struct archive> ext;
    struct archive_entry *entry;

    QMutex mutex;
    bool AskPassword = false; // Default.
    int PasswordTries = 0; // Default.
    int flags = ARCHIVE_EXTRACT_TIME |
                ARCHIVE_EXTRACT_PERM |
                ARCHIVE_EXTRACT_SECURE_NODOTDOT; // default.
    int BlockSize = 10240; // Default BlockSize.
    QString ArchivePath;
    QString Destination;
    QString Password;
    QStringList OnlyExtract;
    UNBlock *UNBlocker = nullptr;


    /*
     * Note that this function can only work
     * if declared in the header.
    */

    /*
     * Password Callback to loop until the
     * correct password , If a empty password
     * is given then just throw a error
     * and quit.
    */
    static const char *password_callback(struct archive *a, void *_client_data)
    {
        (void)a; /* UNUSED */
        Extractor *e = (Extractor*)_client_data;
        if(e->AskPassword) {
            if(e->PasswordTries > 0 || e->Password.isEmpty()) {
                QEventLoop Freeze;
                e->connect(e, SIGNAL(submitPassword()), &Freeze, SLOT(quit()));
                QTimer::singleShot(1000, [e]() {
                    if(e->PasswordTries > 0) {
                        e->ret = ARCHIVE_WRONG_PASSWORD;
                        e->error(ARCHIVE_WRONG_PASSWORD, e->ArchivePath);
                    }
                    emit(e->passwordRequired(e->PasswordTries));
                }); // emit signal.
                Freeze.exec();
                e->disconnect(e, SIGNAL(submitPassword()), &Freeze, SLOT(quit()));
                if(e->Password.isEmpty()) {
                    e->ret = ARCHIVE_PASSWORD_NOT_GIVEN;
                    e->error(ARCHIVE_PASSWORD_NOT_GIVEN, e->ArchivePath);
                    return NULL;
                }
            }
        } else {
            if(e->Password.isEmpty()) {
                e->ret = ARCHIVE_PASSWORD_NOT_GIVEN;
                e->error(ARCHIVE_PASSWORD_NOT_GIVEN, e->ArchivePath);
                return NULL;
            } else if(e->PasswordTries > 0) {
                e->ret = ARCHIVE_WRONG_PASSWORD;
                e->error(ARCHIVE_WRONG_PASSWORD, e->ArchivePath);
                return NULL;
            }
        }
        e->PasswordTries += 1;
        return e->Password.toUtf8().constData();
    }
// ---


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
    SEVEN_ZIP,
    ZIP
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
    Compressor &setArchive(const QString &archive);
    Compressor &setArchive(const QString &archive , const QString &file);
    Compressor &setArchive(const QString &archive , const QStringList &files);
    Compressor &setArchiveFormat(short type);
    Compressor &setPassword(const QString&);
    Compressor &setBlocksize(int);
    Compressor &setCompressionLevel(int);
    Compressor &addFiles(const QString& file);
    Compressor &addFiles(const QStringList& files);
    Compressor &removeFiles(const QString& file);
    Compressor &removeFiles(const QStringList& files);
    Compressor &clear(void);
    ~Compressor();

public Q_SLOTS:
    Compressor &waitForFinished(void);
    Compressor &start(void);
    Compressor &pause(void);
    Compressor &resume(void);
    Compressor &cancel(void);

    bool isRunning() const;
    bool isCanceled() const;
    bool isPaused() const;
    bool isStarted() const;

    Compressor &setFunc(short, std::function<void(void)>);
    Compressor &setFunc(short, std::function<void(QString)>); // compressing and compressed.
    Compressor &setFunc(std::function<void(int)>); // progress bar.
    Compressor &setFunc(std::function<void(short,QString)>); // error.
private Q_SLOTS:
    // The actual Compressor.
    int init(void); // Allocates everything needed.
    int condition(void); // EOF ?
    void expression(void); // Increament.
    int loopContent(void); // Write to archive handle.
    void deinit(int); // clear everything.
    // ------

    // Utils
    QString isDirInFilesList(void);
    void populateDirectory(void);
    void checkNodes(void);
    void getArchiveFormat(void);
Q_SIGNALS:
    void started();
    void finished();
    void paused();
    void resumed();
    void canceled();
    void progress(int);
    void compressing(const QString&);
    void compressed(const QString&);
    void error(short, const QString&);
private:
    int ret = 0;
    QSharedPointer<struct archive> archive;
    QSharedPointer<QSaveFile> tempFile; // Temporary file for the archive.

    QMutex mutex;
    int BlockSize = 10240; // Default BlockSize.
    int CompressionLevel = 0; // Use Default.
    short archiveFormat = NO_FORMAT; // Defaults to gzip.
    QString archivePath;
    QString Password;
    QMap<QString, QString>::iterator mapIter;
    QMap<QString, QString> nodes;  // (1)-> File path , (2)-> entry in archive.
    UNBlock *UNBlocker = nullptr;
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
    explicit Reader(const QString&);
    Reader &setArchive(const QString&);
    Reader &setPassword(const QString&);
    Reader &setAskPassword(bool);
    Reader &setBlocksize(int);
    QJsonObject getFilesList(void);
    Reader &clear();
    ~Reader();

public Q_SLOTS:
    Reader &waitForFinished(void);
    Reader &start(void);
    Reader &pause(void);
    Reader &resume(void);
    Reader &cancel(void);

    bool isRunning() const;
    bool isCanceled() const;
     bool isPaused() const;
     bool isStarted() const;

     Reader &setFunc(short, std::function<void(void)>);
     Reader &setFunc(std::function<void(int)>); // Password Required.
     Reader &setFunc(std::function<void(QJsonObject)>); // files list.
     Reader &setFunc(std::function<void(short,QString)>); // error.
private Q_SLOTS:
    int init(void);
    int condition(void);
    int loopContent(void);
    void deinit(int);

    QString getDirectoryFileName(const QString&);

Q_SIGNALS:
    void started();
    void finished();
    void paused();
    void resumed();
    void canceled();
    void passwordRequired(int);
    void submitPassword();
    void filesList(QJsonObject);
    void error(short, const QString&);

private:
    int ret = 0;
    QSharedPointer<struct archive> archive;
    struct archive_entry *entry;

    QMutex mutex;
    bool AskPassword = false; // Default.
    int PasswordTries = 0; // Default.
    int BlockSize = 10240; // Default BlockSize.
    QString ArchivePath;
    QString Password;
    QJsonObject ArchiveContents;
    UNBlock *UNBlocker = nullptr;

    /*
     * Note that this function can only work
     * if declared in the header.
    */

    /*
     * Password Callback to loop until the
     * correct password , If a empty password
     * is given then just throw a error
     * and quit.
    */
    static const char *password_callback(struct archive *a, void *_client_data)
    {
        
        (void)a; /* UNUSED */
        Reader *e = (Reader*)_client_data;
        if(e->AskPassword) {
            if(e->PasswordTries > 0 || e->Password.isEmpty()) {
                QEventLoop Freeze;
                e->connect(e, SIGNAL(submitPassword()), &Freeze, SLOT(quit()));
                QTimer::singleShot(1000, [e]() {
                    if(e->PasswordTries > 0) {
                        e->ret = ARCHIVE_WRONG_PASSWORD;
                        e->error(ARCHIVE_WRONG_PASSWORD, e->ArchivePath);
                    }
                    emit(e->passwordRequired(e->PasswordTries));
                }); // emit signal.
                Freeze.exec();
                e->disconnect(e, SIGNAL(submitPassword()), &Freeze, SLOT(quit()));
                if(e->Password.isEmpty()) {
                    e->ret = ARCHIVE_PASSWORD_NOT_GIVEN;
                    e->error(ARCHIVE_PASSWORD_NOT_GIVEN, e->ArchivePath);
                    return NULL;
                }
            }
        } else {
            if(e->Password.isEmpty()) {
                e->ret = ARCHIVE_PASSWORD_NOT_GIVEN;
                e->error(ARCHIVE_PASSWORD_NOT_GIVEN, e->ArchivePath);
                return NULL;
            } else if(e->PasswordTries > 0) {
                e->ret = ARCHIVE_WRONG_PASSWORD;
                e->error(ARCHIVE_WRONG_PASSWORD, e->ArchivePath);
                return NULL;
            }
        }
        e->PasswordTries += 1;
        return e->Password.toUtf8().constData();
    }
// ---
}; // Class Reader Ends


} // QArchive Namespace Ends.
#endif // QARCHIVE_HPP_INCLUDED
