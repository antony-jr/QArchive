#include <QCoreApplication>
#include <QDateTime>
#include <QFileInfo>
#include <QVector>
#include <QDir>

#include <qarchivediskcompressor_p.hpp>
#include <qarchiveutils_p.hpp>
#include <qarchive_enums.hpp>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
}

using namespace QArchive;

DiskCompressorPrivate::DiskCompressorPrivate()
	: QObject()
{
	m_TemporaryFile.reset(new QSaveFile);
	m_StaggedFiles.reset(new QLinkedList<QPair<QString , QString>>);
	m_ConfirmedFiles.reset(new QLinkedList<QPair<QString , QString>>);
}

DiskCompressorPrivate::~DiskCompressorPrivate()
{
}

void DiskCompressorPrivate::setFileName(const QString &fileName)
{
	if(b_Started || b_Paused){
		return;
	}
	m_ArchivePath = fileName; /* later will be validated for permissions. */
	return;
}

void DiskCompressorPrivate::setArchiveFormat(short format)
{
	if(b_Started || b_Paused){
		return;
	}
	m_ArchiveFormat = format;
	return;	
}

void DiskCompressorPrivate::setPassword(const QString &passwd)
{
	if(b_Started || b_Paused){
		return;
	}
	m_Password = passwd;
	return;
}

void DiskCompressorPrivate::setBlockSize(int size)
{
	if(b_Started || b_Paused){
		return;
	}
	n_BlockSize = size;
	return;
}

void DiskCompressorPrivate::addFiles(const QString &file)
{
	if(b_Started || b_Paused){
		return;
	}	
	QPair<QString , QString> node(file , file);
	m_StaggedFiles->append(node);
	return;
}

void DiskCompressorPrivate::addFiles(const QStringList &files)
{
	if(b_Started || b_Paused){
		return;
	}
	for(auto i = 0; i < files.size(); ++i){
		QPair<QString , QString> node(files.at(i) , files.at(i));
		if(m_StaggedFiles->contains(node)){
			continue;
		}
		m_StaggedFiles->append(node);
	}
	return;
}

void DiskCompressorPrivate::addFiles(const QString &entryName , const QString &file)
{
	if(b_Started || b_Paused){
		return;
	}
	QPair<QString , QString> node(entryName , file);
	if(m_StaggedFiles->contains(node)){
		return;
	}
	m_StaggedFiles->append(node);
	return;
}

void DiskCompressorPrivate::addFiles(const QStringList &entryNames , const QStringList &files)
{
	if(b_Started || b_Paused){
		return;
	}
	if(entryNames.size() != files.size()){
		return;
	}
	for(auto i = 0; i < files.size(); ++i){
		QPair<QString , QString> node(entryNames.at(i) , files.at(i));
		if(m_StaggedFiles->contains(node)){
			continue;
		}
		m_StaggedFiles->append(node);
	}
	return;
}

void DiskCompressorPrivate::removeFiles(const QString &file)
{
	if(b_Started || b_Paused){
		return;
	}
	QPair<QString , QString> node(file , file);
	m_StaggedFiles->removeAll(node);
	return;
}

void DiskCompressorPrivate::removeFiles(const QStringList &files)
{
	if(b_Started || b_Paused){
		return;
	}
	for(auto i = 0; i < files.size() ; ++i)
	{
		QPair<QString , QString> node(files.at(i) , files.at(i));
		m_StaggedFiles->removeAll(node);
	}
	return;
}


void DiskCompressorPrivate::removeFiles(const QString &entryName , const QString &file)
{
	if(b_Started || b_Paused){
		return;
	}
	QPair<QString , QString> node(entryName , file);
	m_StaggedFiles->removeAll(node);
	return;
}

void DiskCompressorPrivate::removeFiles(const QStringList &entryNames , const QStringList &files)
{
	if(b_Started || b_Paused){
		return;
	}
	if(entryNames.size() != files.size()){
		return;
	}

	for(auto i = 0; i < files.size() ; ++i)
	{
		QPair<QString , QString> node(entryNames.at(i) , files.at(i));
		m_StaggedFiles->removeAll(node);
	}
	return;
}

void DiskCompressorPrivate::listStaggedFiles()
{
	if(b_Started || b_Paused || m_StaggedFiles->isEmpty()){
		return;
	}
	emit staggedFiles(m_StaggedFiles.data());
	return;
}

void DiskCompressorPrivate::clear()
{
	if(b_Started){
		return;
	}
	b_PauseRequested = b_CancelRequested = b_Paused = b_Started = b_Finished = false;
	m_ArchivePath.clear();
	m_Password.clear(); 

	m_ArchiveFormat = NoFormat;
	n_BlockSize = 0;

	m_ConfirmedFiles->clear();
	m_StaggedFiles->clear();

	m_TemporaryFile.reset(new QSaveFile);
}

void DiskCompressorPrivate::start()
{
	if(b_Started || b_Paused){
		return;
	}else if(m_TemporaryFile->fileName().isEmpty()){
		emit error(ArchiveFileNameNotGiven , QString());
		return;
	}else if(QFileInfo::exists(m_TemporaryFile->fileName())){
		emit error(ArchiveFileAlreadyExists , m_TemporaryFile->fileName());
		return;	
	}else if(m_StaggedFiles->isEmpty()){
		emit error(NoFilesToCompress , m_TemporaryFile->fileName());
		return;
	}else{
		/* Guess Archive Format if not given. */
		if(m_ArchiveFormat == NoFormat){
		if(!guessArchiveFormat()){
			m_ArchiveFormat = ZipFormat; /* Default format. */
		}
		}

		/* Confirm files. */
		if(!confirmFiles()){
			return;
		}
	}

	b_Started = true;
	emit started();

	short ret = compress();
	if(ret == NoError){
		b_Started = false;
		b_Finished = true;
		emit finished();
	}else if(ret == OperationCanceled){
		b_Started = false;
		emit canceled();
	}else if(ret == OperationPaused){
		b_Started = false;
		b_Paused = true;
		emit paused();
	}
	return;
}

void DiskCompressorPrivate::resume()
{
	if(!b_Paused || b_Finished){
		return;
	}
	b_Paused = false;
	b_Started = true;
	emit resumed();

	short ret = compress();
	if(ret == NoError){
		b_Started = false;
		b_Finished = true;
		emit finished();
	}else if(ret == OperationCanceled){
		b_Started = false;
		emit canceled();
	}else if(ret == OperationPaused){
		b_Started = false;
		b_Paused = true;
		emit paused();
	}
	return;
}

void DiskCompressorPrivate::pause()
{
	if(!b_Started || b_Finished || b_Paused){
		return;
	}
	b_PauseRequested = true;
	return;
}

void DiskCompressorPrivate::cancel()
{
	if(!b_Started || b_Finished || b_Paused){
		return;
	}
        b_CancelRequested = true;
	return;
}

/*
 * Guesses the archive format from the given archive filename , on success
 * this returns true or vice-versa.
*/
bool DiskCompressorPrivate::guessArchiveFormat()
{
	if(m_TemporaryFile->fileName().isEmpty()){
		return false;
	}
	
	auto ext = QFileInfo(m_TemporaryFile->fileName()).suffix().toLower();

    if(ext == "bz") {
	    m_ArchiveFormat = BZipFormat;
    } else if(ext == "bz2") {
	    m_ArchiveFormat = BZip2Format;
    } else if(ext == "gz") {
	    m_ArchiveFormat = GZipFormat;
    } else if(ext == "xar") {
	    m_ArchiveFormat = XarFormat;
    } else if(ext == "zip") {
	    m_ArchiveFormat = ZipFormat;
    } else if(ext == "7z") {
	    m_ArchiveFormat = SevenZipFormat;
    } else {
	    m_ArchiveFormat = NoFormat;
	    return false;
    }

    return true;
}

/*
 * Confirms all the files that are stagged for compression , Returns true 
 * on success and vice-versa. 
 * This populates m_ConfirmedFiles linked list with all the files added ,
 * Directory's files will be recursively added.
*/
bool DiskCompressorPrivate::confirmFiles()
{
	m_ConfirmedFiles->clear();
	for(auto iter = m_StaggedFiles->begin() ; iter != m_StaggedFiles->end() ; ++iter)
	{
		auto node = *iter;
		QFileInfo info(/* file path given by the user = */node.second);
		
		/* Check if the file exists. */
		if(info.exists()){
			emit error(FileDoesNotExist , info.filePath());
			return false;
		}

		/* Check permission to read. */
		if(!info.isReadable()){
			emit error(NoPermissionToReadFile , info.filePath());
			return false;
		}


		/* Check if it is file or a directory ,
		 * if directory then add files in directory 
		 * recursively.
		*/
		if(info.isDir()){
			QVector<QString> dirList;
			dirList.append(info.filePath());
			bool replace = (node.first != node.second);
			
			while(!dirList.isEmpty()){
			QDir dir(dirList.takeFirst());
			QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
			for (int i = 0; i < list.size(); i++) {
				if(list.at(i).isDir()){
					dirList.append(list.at(i).filePath());
					continue;
				}
				QString file = list.at(i).filePath();
				QPair<QString , QString> fileNode;
				if(replace){
					fileNode.first = file.replace(node.second , node.first);
				}else{
					fileNode.first = file;
				}
				fileNode.second = file;
				m_ConfirmedFiles->append(fileNode);
			}
			}
		}else{ /* Add it to the confirmed list. */
			if(node.first == node.second){
				node.first = info.filePath();
			}
			node.second = info.filePath();
			m_ConfirmedFiles->append(node);
		}
	}
	return true;
}

short DiskCompressorPrivate::compress()
{
	if(m_ArchiveWrite.isNull()){
		/* Open Temporary file for write. */
		if(!m_TemporaryFile->open(QIODevice::WriteOnly)) {
			emit error(ArchiveWriteOpenError, m_TemporaryFile->fileName());
			return ArchiveWriteOpenError;
		}

		/* Allocate Archive Write. */
		m_ArchiveWrite = QSharedPointer<struct archive>(archive_write_new() , ArchiveWriteDestructor);
		if(m_ArchiveWrite.isNull()){
			emit error(NotEnoughMemory , m_TemporaryFile->fileName());
			return NotEnoughMemory;
		}

		/* Set format for the archive. */
		switch (m_ArchiveFormat) {
		case BZipFormat:
		case BZip2Format:
			archive_write_set_format_ustar(m_ArchiveWrite.data());
			archive_write_add_filter_bzip2(m_ArchiveWrite.data());
			break;
		case GZipFormat:
			archive_write_set_format_ustar(m_ArchiveWrite.data());
			archive_write_add_filter_gzip(m_ArchiveWrite.data());	
			break;
		case XarFormat:
			archive_write_set_format_xar(m_ArchiveWrite.data());
			break;
		case SevenZipFormat:
			archive_write_set_format_7zip(m_ArchiveWrite.data());
			break;
		case ZipFormat:
		default:
			archive_write_set_format_zip(m_ArchiveWrite.data());
			break;
		}

		/* Set Password if the format is Zip and a password is given by the user.
		 * 
		 * Note:
		 * Currently only Zip format is officially supported by libarchive for the 
		 * ability to use passwords and thus until the user uses Zip format , the 
		 * password even if given is ignored.
		*/
		if(!m_Password.isEmpty() && m_ArchiveFormat == ZipFormat) {
			archive_write_set_passphrase(m_ArchiveWrite.data(), m_Password.toUtf8().constData());
			archive_write_set_options(m_ArchiveWrite.data() , "zip:encryption=traditional");
		}

		/* Set explicit blocksize if the user gives one. */
		if(n_BlockSize){
			archive_write_set_bytes_per_block(m_ArchiveWrite.data(), n_BlockSize);
		}

		/* Finally open the write archive using the handle of the Temporary file. */
		if(archive_write_open_fd(m_ArchiveWrite.data(), m_TemporaryFile->handle()) != ARCHIVE_OK){
			m_ArchiveWrite.clear();
			emit error(ArchiveWriteOpenError , m_TemporaryFile->fileName());
			return ArchiveWriteOpenError;
		}

		n_TotalEntries = m_ConfirmedFiles->size(); /* for reporting progress. */
	}

	/* Start compressing files. */
	while(!m_ConfirmedFiles->isEmpty()){
		auto node = m_ConfirmedFiles->takeFirst();
		
		/*
		 * TODO:
		 * 	Implement a failsafe copy mechanism.
		 *
		 * Note:
		 * Down below implementation is nearly good but
		 * not very robust and thus needs a good implementation.
		*/
		int r;
		ssize_t len;
		char buff[16384]; /* Default buffer size */
		
		auto disk = QSharedPointer<struct archive>(archive_read_disk_new(), ArchiveReadDestructor);
		archive_read_disk_set_standard_lookup(disk.data());
		
		r = archive_read_disk_open(disk.data(), QFile::encodeName(/*file path = */node.second).constData());
		if(r != ARCHIVE_OK) {
			emit error(DiskOpenError, node.second);
			return DiskOpenError;
		}
		
		for (;;) {
			auto entry = QSharedPointer<struct archive_entry>(archive_entry_new(), ArchiveEntryDestructor);
			r = archive_read_next_header2(disk.data(), entry.data());
			if (r == ARCHIVE_EOF) {
				break;
			}
			if (r != ARCHIVE_OK) {
				emit error(DiskReadError, node.second);
				return DiskReadError;
			}
			archive_read_disk_descend(disk.data());
			archive_entry_set_pathname(entry.data(), (node.first).toUtf8().constData());
			r = archive_write_header(m_ArchiveWrite.data(), entry.data());
			if (r == ARCHIVE_FATAL) {
				emit error(ArchiveFatalError, node.second);
				return ArchiveFatalError;
			}
			if (r > ARCHIVE_FAILED) {
				QScopedPointer<QFile> file(new QFile(node.second));
				if(!file->open(QIODevice::ReadOnly)) {
					emit error(DiskOpenError, node.second);
					return DiskOpenError;
				}
				len = file->read(buff, sizeof(buff));
				while (len > 0) {
					archive_write_data(m_ArchiveWrite.data(), buff, len);
					len = file->read(buff, sizeof(buff));
				}
				file->close();
			}
		}
		
                emit progress(QString(node.second),
                              m_ConfirmedFiles->size(),
                              n_TotalEntries,
                              (m_ConfirmedFiles->size()*100)/n_TotalEntries);
       

            QCoreApplication::processEvents();
            if(b_PauseRequested) {
                b_PauseRequested = false;
                return OperationPaused;
            }

            if(b_CancelRequested) {
                b_CancelRequested = false;
                m_ConfirmedFiles->clear();
		m_ArchiveWrite.clear();
                return OperationCanceled;
            }
	}
	return NoError;
}
