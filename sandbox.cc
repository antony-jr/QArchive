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
	auto metaObject = e.metaObject();
	QObject::connect(&e , &DiskExtractorPrivate::info ,[&](QJsonObject info){
			qDebug() << info;
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::finished , [&](){
			app.quit();
			return;
	});

	QObject::connect(&e , &DiskExtractorPrivate::error , [&](short code){
			qDebug() << "ERROR CODE: " << code;
			app.quit();
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
		metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("getInfo()")))
			   .invoke(&e , Qt::QueuedConnection);
		metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature("start()")))
			   .invoke(&e , Qt::QueuedConnection);
	});
	return app.exec();
}
