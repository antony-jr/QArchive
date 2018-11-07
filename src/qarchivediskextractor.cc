#include <QMetaMethod>
#include <QMetaObject>

#include <qarchivediskextractor_p.hpp>
#include <qarchivediskextractor.hpp>

using namespace QArchive;

#define CONSTRUCT(code) _mExtractor.reset(new DiskExtractorPrivate); \
			if(!singleThreaded){ \
				_mThread.reset(new QThread);\
				_mExtractor->moveToThread(_mThread.data());\
			} \
			connect(_mExtractor.data() , &DiskExtractorPrivate::started ,\
				this , &DiskExtractor::started , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::canceled ,\
				this , &DiskExtractor::canceled , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::paused ,\
				this , &DiskExtractor::paused , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::resumed ,\
				this , &DiskExtractor::resumed , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::finished ,\
				this , &DiskExtractor::finished , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::error ,\
				this , &DiskExtractor::error , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::progress ,\
				this , &DiskExtractor::progress , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::getInfoRequirePassword ,\
				this , &DiskExtractor::getInfoRequirePassword , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::extractionRequirePassword ,\
				this , &DiskExtractor::extractionRequirePassword , Qt::DirectConnection); \
			connect(_mExtractor.data() , &DiskExtractorPrivate::info,\
				this , &DiskExtractor::info , Qt::DirectConnection); \
			code

static QMetaMethod getMethod(DiskExtractorPrivate *o, const char *mId)
{
    auto metaObject = o->metaObject();
    return metaObject->method(metaObject->indexOfMethod(QMetaObject::normalizedSignature(mId)));
}

DiskExtractor::DiskExtractor(QObject *parent, bool singleThreaded)
    : QObject(parent)
{
    CONSTRUCT();
}

DiskExtractor::DiskExtractor(QFile *archive, bool singleThreaded)
    : QObject()
{
    CONSTRUCT(
        setArchive(archive);
    );
}

DiskExtractor::DiskExtractor(const QString &archivePath, bool singleThreaded)
    : QObject()
{
    CONSTRUCT(
        setArchive(archivePath);
    );
}

DiskExtractor::~DiskExtractor()
{
    return;
}

void DiskExtractor::setArchive(QFile *archive)
{
    getMethod(_mExtractor.data(),
              "setArchive(QFile*)").invoke(_mExtractor.data(),
                                           Qt::QueuedConnection,
                                           Q_ARG(QFile*, archive));
    return;
}

void DiskExtractor::setArchive(const QString &archivePath)
{
    getMethod(_mExtractor.data(),
              "setArchive(const QString&)").invoke(_mExtractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, archivePath));
    return;
}

void DiskExtractor::setBlockSize(int n)
{
    getMethod(_mExtractor.data(),
              "setBlockSize(int)").invoke(_mExtractor.data(),
                                          Qt::QueuedConnection,
                                          Q_ARG(int, n));
    return;
}

void DiskExtractor::setShowProgress(bool choice)
{
    getMethod(_mExtractor.data(),
              "setShowProgress(bool)").invoke(_mExtractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(bool, choice));
    return;
}

void DiskExtractor::setOutputDirectory(const QString &dir)
{
    getMethod(_mExtractor.data(),
              "setOutputDirectory(const QString&)").invoke(_mExtractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, dir));
    return;
}

void DiskExtractor::setPassword(const QString &passwd)
{
    getMethod(_mExtractor.data(),
              "setPassword(const QString&)").invoke(_mExtractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, passwd));
    return;
}

void DiskExtractor::addFilter(const QString &filter)
{
    getMethod(_mExtractor.data(),
              "addFilter(const QString&)").invoke(_mExtractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QString, filter));
    return;
}

void DiskExtractor::addFilter(const QStringList &filters)
{
    getMethod(_mExtractor.data(),
              "addFilter(const QStringList&)").invoke(_mExtractor.data(),
                      Qt::QueuedConnection,
                      Q_ARG(QStringList, filters));
    return;
}

void DiskExtractor::clear()
{
    getMethod(_mExtractor.data(), "clear()").invoke(_mExtractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::getInfo()
{
    getMethod(_mExtractor.data(), "getInfo()").invoke(_mExtractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::start()
{
    getMethod(_mExtractor.data(), "start()").invoke(_mExtractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::cancel()
{
    getMethod(_mExtractor.data(), "cancel()").invoke(_mExtractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::pause()
{
    getMethod(_mExtractor.data(), "pause()").invoke(_mExtractor.data(), Qt::QueuedConnection);
    return;
}

void DiskExtractor::resume()
{
    getMethod(_mExtractor.data(), "resume()").invoke(_mExtractor.data(), Qt::QueuedConnection);
    return;
}

