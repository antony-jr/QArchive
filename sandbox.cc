#include <QCoreApplication>
#include <QDebug>
#include <QVector>
#include <QMetaObject>
#include <QMetaMethod>
#include <QJsonObject>
#include <QtConcurrent>
#include <QTimer>
#include <qarchivediskextractor_p.hpp>
#include <iostream>
#include <string>

int main(int ac , char **av)
{
	if(ac == 1)
		return -1;

	using QArchive::DiskExtractorPrivate;
	QCoreApplication app(ac , av);
	QString archive(av[1]);
	DiskExtractorPrivate e(archive);
	e.setShowProgress(true);
	e.setOutputDirectory("Output/");
	auto metaObject = e.metaObject();
	QObject::connect(&e , &DiskExtractorPrivate::info ,[&](QJsonObject info){
			qDebug() << info;
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::finished , [&](){
			app.quit();
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::started , [&](){
			QTimer::singleShot(1000 , [&](){
			metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("pause()")))
			   .invoke(&e , Qt::QueuedConnection);		
			 });
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::resumed , [&](){
			qDebug() << "RESUMED!";
	});

	QObject::connect(&e , &DiskExtractorPrivate::paused , [&](){
			qDebug() << "PAUSED!";
			QTimer::singleShot(5000 , [&](){
			metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("resume()")))
			   .invoke(&e , Qt::QueuedConnection);		
			 });
			return;
	});



	QObject::connect(&e , &DiskExtractorPrivate::error , [&](short code){
			qDebug() << "ERROR CODE: " << code;
			app.quit();
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::progress , [&](QString file , int d , int t , int p){
			(void)d;
			(void)t;
			qDebug() << "Extracting: " << file << " , Done: " << p << " %.";
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::extractionRequirePassword , [&](int tried){
			using std::cin;	
			using std::string;
			using std::cout;
			string pwd;
			cout << "Enter Password(Tries = " << tried << "): ";
			cin >> pwd;
			e.setPassword(QString::fromStdString(pwd));
			metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("start()")))
			   .invoke(&e , Qt::QueuedConnection);	
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::getInfoRequirePassword , [&](int tried){
			using std::cin;	
			using std::string;
			using std::cout;
			string pwd;
			cout << "Enter Password(Tries = " << tried << "): ";
			cin >> pwd;
			e.setPassword(QString::fromStdString(pwd));
			metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("getInfo()")))
			   .invoke(&e , Qt::QueuedConnection);
			return;
	});

	QTimer::singleShot(0 , [&](){
		metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("start()")))
			   .invoke(&e , Qt::QueuedConnection);
	});
	return app.exec();
}
