#include <QCoreApplication>
#include <QDebug>
#include "QArchive.hpp"

int main(int argc , char** argv)
{
	QCoreApplication app(argc , argv);

	/*
	 * 1.Construct
	*/
	QArchive::Extractor e("test.7z" , "Output");
	//		Archive  -^	    ^- Extract to Directory


	/*
	 * 2.Connect Callbacks
	*/

	// emitted when all extraction is finished 
	QObject::connect(&e , &QArchive::Extractor::finished , [&](){
		qDebug() << "Finished all extraction!";
		app.quit();		
	});

	QObject::connect(&e , &QArchive::Extractor::extracting , [&](QString file){
		qDebug() << "Extracting:: " << file;
	});

	QObject::connect(&e , &QArchive::Extractor::status , [&](QString archive , QString stat){
		qDebug() << archive << "::" << stat;
	});

	// emitted when a file is extracted
	QObject::connect(&e , &QArchive::Extractor::extracted , [&](QString file){
		qDebug() << "Extracted:: " << file;
	});

	// emitted when something goes wrong
	QObject::connect(&e , &QArchive::Extractor::error , [&](short code , QString file){
		switch(code){
		case QArchive::ARCHIVE_READ_ERROR:
				qDebug() << "unable to find archive :: " << file;
				app.quit();
				break;
		case QArchive::ARCHIVE_QUALITY_ERROR:
				qDebug() << "bad archive! :: " << file;
				app.quit();
				break;
		case QArchive::ARCHIVE_UNCAUGHT_ERROR:
				qDebug() << "fatal error. :: " << file;
				app.quit();
				break;
		case QArchive::INVALID_DEST_PATH:
				qDebug() << "cannot find the destination path!";
				app.quit();
				break;
		default:
				qDebug() << "unknown error. :: " << file;
				app.quit();
				break;
		}
	});

	/*
	 * 3.Start extraction!
	*/
	e.start();

	qDebug() << "Will be extracted in the place you think!";

	return app.exec();
}
