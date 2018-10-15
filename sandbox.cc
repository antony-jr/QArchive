#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
#include <QTimer>
#include <qarchivediskextractor_p.hpp>

int main(int ac , char **av)
{
	if(ac == 1)
		return -1;

	using QArchive::DiskExtractorPrivate;
	QCoreApplication app(ac , av);
	QString archive(av[1]);
	DiskExtractorPrivate e(archive);
	QObject::connect(&e , &DiskExtractorPrivate::info ,[&](QJsonObject info){
			qDebug() << info;
	});
	QTimer::singleShot(0, [&](){
		e.getInfo();
		app.quit();
	});
	return app.exec();
}
