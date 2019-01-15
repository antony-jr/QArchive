#ifndef QARCHIVE_DISK_COMPRESSOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_DISK_COMPRESSOR_PRIVATE_HPP_INCLUDED
#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QHash>
#include <QScopedPointer>

namespace QArchive {
class DiskCompressorPrivate : public QObject {
	Q_OBJECT
public:
	DiskCompressorPrivate();
	~DiskCompressorPrivate();
public Q_SLOTS:
	void setArchive(const QString&);
	void setArchive(QFile*);
	void setArchiveFormat(short);
	void setPassword(const QString&);
	void setCompression(const QString&);
	void setCompressionLevel(int);
	void setBlockSize(int);
	void addFiles(const QString&);
	void addFiles(const QStringList&);
	void addFiles(const QString& , const QString&);
	void addFiles(const QStringList& , const QStringList&);
	void removeFiles(const QString&);
	void removeFiles(const QStringList&);
	
	void listStaggedFiles();

	void start();
	void cancel();
	void pause();
	void resume();
Q_SIGNALS:
	void staggedFiles(QStringList);
	void error(short);
	void started();
	void canceled();
	void paused();
	void resumed();
private:
	bool b_PauseRequested = false,
	     b_CancelRequested = false,
	     b_Paused = false,
	     b_Started = false,
	     b_Finished = false;
	
	QString m_ArchivePath,
		/* Down below are only used for ZIP archives. */
		m_Password, 
		m_Compression;

	QFile *m_Archive = nullptr;
	short m_ArchiveFormat = NoFormat; /* Defaults to ZIP. */
	int n_CompressionLevel = 0, /* Only used for GZIP. */
	    n_BlockSize = 10240;
	QScopedPointer<QHash<QString , QString>> m_StaggedFiles;
};
}
#endif // QARCHIVE_DISK_COMPRESSOR_PRIVATE_HPP_INCLUDED
