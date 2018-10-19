#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
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
	QObject::connect(&e , &DiskExtractorPrivate::info ,[&](QJsonObject info){
			qDebug() << info;
	});
	QObject::connect(&e , &DiskExtractorPrivate::passwordRequired , [&](int tried){
			using std::cin;	
			using std::string;
			using std::cout;
			string pwd;
			cout << "Enter Password(Tries = " << tried << "): ";
			cin >> pwd;
			e.setPassword(QString::fromStdString(pwd));
			return;
	});
	QTimer::singleShot(0, [&](){
		e.getInfo();
		app.quit();
	});
	return app.exec();
}
