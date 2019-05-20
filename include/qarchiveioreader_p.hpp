#ifndef QARCHIVE_IO_READER_PRIVATE_HPP_INCLUDED
#define QARCHIVE_IO_READER_PRIVATE_HPP_INCLUDED
#include <QIODevice>
#include <QObject>
#include <QtGlobal>

namespace QArchive {
class IOReaderPrivate {
  public:
    IOReaderPrivate();
    IOReaderPrivate(const IOReaderPrivate&);
    ~IOReaderPrivate();

    void setBlockSize(int);
    void setIODevice(QIODevice*);

    bool isOpen();
    bool isReadable();
    bool isSequential();

    qint64 read(char*);
    qint64 seek(qint64, /*whence=*/int);
    qint64 skip(qint64);

    void operator = (const IOReaderPrivate&);
  private:
    qint64 n_FilePointerPos = 0;
    int n_BlockSize = 10204; /* Default. */
    QIODevice *m_IODevice = nullptr;
};
}

#endif // QARCHIVE_IO_READER_PRIVATE_HPP_INCLUDED
