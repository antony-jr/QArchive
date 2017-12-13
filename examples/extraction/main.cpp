#include <QCoreApplication>
#include <QDebug>
#include "QArchive.hpp"

int main(int argc , char** argv)
{
	QCoreApplication app(argc , argv);

	QString file("test.7z");

	QArchive::Extractor *e = new QArchive::Extractor(file);

	QObject::connect(e , &QArchive::Extractor::finished , [&](){
		qDebug() << "finished extraction!";
		app.quit();		
	});

	e->start();

	qDebug() << "Non-Blocking too... ";

	return app.exec();
}
