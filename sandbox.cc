#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
#include <QTimer>
#include <iostream>
#include <string>
#include <qarchivediskextractor.hpp>

int main(int ac , char **av)
{
	if(ac == 1)
		return -1;
	using QArchive::DiskExtractor;
	QCoreApplication app(ac , av);
	QString archive(av[1]);
	DiskExtractor e(archive);
	QObject::connect(&e , &DiskExtractor::info ,[&](QJsonObject info){
			qDebug() << info;
			return;
	});

	QObject::connect(&e , &DiskExtractor::finished , [&](){
			app.quit();
			return;
	});

	QObject::connect(&e , &DiskExtractor::started , [&](){
			QTimer::singleShot(1000 , [&](){
				e.pause();
			});	
			return;
	});

	QObject::connect(&e , &DiskExtractor::resumed , [&](){
			qDebug() << "RESUMED!";
	});

	QObject::connect(&e , &DiskExtractor::paused , [&](){
			qDebug() << "PAUSED!";
			QTimer::singleShot(5000 , [&](){
				e.resume();
			});
			return;
	});



	QObject::connect(&e , &DiskExtractor::error , [&](short code){
			qDebug() << "ERROR CODE: " << code;
			app.quit();
			return;
	});

	QObject::connect(&e , &DiskExtractor::progress , [&](QString file , int d , int t , int p){
			(void)d;
			(void)t;
			qDebug() << "Extracting: " << file << " , Done: " << p << " %.";
			return;
	});

	QObject::connect(&e , &DiskExtractor::extractionRequirePassword , [&](int tried){
			using std::cin;	
			using std::string;
			using std::cout;
			string pwd;
			cout << "Enter Password(Tries = " << tried << "): ";
			cin >> pwd;
			e.setPassword(QString::fromStdString(pwd));
			e.start();
			return;
	});

	QObject::connect(&e , &DiskExtractor::getInfoRequirePassword , [&](int tried){
			using std::cin;	
			using std::string;
			using std::cout;
			string pwd;
			cout << "Enter Password(Tries = " << tried << "): ";
			cin >> pwd;
			e.setPassword(QString::fromStdString(pwd));
			e.getInfo();
			return;
	});
	QTimer::singleShot(0 , [&](){
	e.start();
	});
	return app.exec();
}
