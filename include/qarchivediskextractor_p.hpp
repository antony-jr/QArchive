#ifndef QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#include <QFile>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include <QJsonObject>

struct archive;
struct archive_entry;

namespace QArchive {
enum : short {
	ArchiveDoesNotExists,
	ArchiveReadError,
	ArchiveCorrupted,
	ArchiveIsNotReadable,
	ArchiveIsNotOpened,
	CannotOpenArchive,
	NoPermissionToReadArchive,
	NoPermissionToWrite,
	InvalidOutputDirectory,
	InvalidQFile,
	NotEnoughMemory

};

class DiskExtractorPrivate : public QObject {
	Q_OBJECT
	public:
		DiskExtractorPrivate();
		DiskExtractorPrivate(QFile*);
		DiskExtractorPrivate(const QString&);
		~DiskExtractorPrivate();
	public Q_SLOTS:
		void setArchive(QFile*);
		void setArchive(const QString&);
		void setBlockSize(int);
		void setOutputDirectory(const QString&);
		void setPassword(const QString&);
		void setPasswordTryLimit(int);
		void addFilter(const QString&);
		void addFilter(const QStringList&);
		void clear();

		void getInfo();
	//	void start();
	//	void cancel();
	//	void pause();
	//	void resume();
	Q_SIGNALS:
		void error(short);
		void progress(int);
		void info(QJsonObject);
		void started();
		void canceled();
		void paused();
		void resumed();
		void finished();
		void passwordRequired(QString * , int);
	private:
		QSharedPointer<archive> _mInArchive = nullptr,
					_mOutArchive = nullptr;
		int _nPasswordTryLimit = 3,
		    _nBlockSize = 10240;
		QString _mOutputDirectory,
			_mPassword;
		QJsonObject _mInfo;
		QSharedPointer<QStringList> _mExtractFilters = nullptr;
		QSharedPointer<QFile> _mArchive = nullptr;
};
}
#endif // QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
