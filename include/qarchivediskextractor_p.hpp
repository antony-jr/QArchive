#ifndef QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#define QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
#include <QFile>
#include <QObject>
#include <QString>
#include <QScopedPointer>
#include <QJsonObject>

namespace QArchive {
class DiskExtractorPrivate : public QObject {
	public:
		DiskExtractorPrivate();
		DiskExtractorPrivate(QFile*);
		DiskExtractorPrivate(const QString&);
		~DiskExtractorPrivate();
	public Q_SLOTS:
		void setArchive(QFile*);
		void setArchive(const QString&);
		void setOutputDirectory(const QString&);
		void setPassword(const QString&);
		void setPasswordTryLimit(int);

		void getInfo();
		void start();
		void cancel();
		void pause();
		void resume();
	Q_SIGNALS:
		void info(QJsonObject);
		void started();
		void canceled();
		void paused();
		void resumed();
		void finished();
		void passwordRequired(QString * , int);
	private:
		int _nPasswordTryLimit = 3;
		QScopedPointer<QFile> _mArchive;
};
}
#endif // QARCHIVE_DISK_EXTRACTOR_PRIVATE_HPP_INCLUDED
