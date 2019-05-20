#include <qarchiveioreader_p.hpp>

using namespace QArchive;

IOReaderPrivate::IOReaderPrivate() {
    return;
}

IOReaderPrivate::IOReaderPrivate(const IOReaderPrivate &copy)
    : n_FilePointerPos(copy.n_FilePointerPos),
      n_BlockSize(copy.n_BlockSize),
      m_IODevice(copy.m_IODevice) {
    return;
}

IOReaderPrivate::~IOReaderPrivate() {
    return;
}


void IOReaderPrivate::operator = (const IOReaderPrivate &copy) {
    n_FilePointerPos = copy.n_FilePointerPos;
    n_BlockSize = copy.n_BlockSize;
    m_IODevice = copy.m_IODevice;
    return;
}

void IOReaderPrivate::setBlockSize(int blockSize) {
    n_BlockSize = (blockSize < 1024) ?
                  10204 : blockSize;
    return;
}

void IOReaderPrivate::setIODevice(QIODevice *io) {
    m_IODevice = io;
    return;
}

bool IOReaderPrivate::isOpen() {
    if(m_IODevice) {
        return m_IODevice->isOpen();
    }
    return false;
}

bool IOReaderPrivate::isReadable() {
    if(m_IODevice) {
        return m_IODevice->isReadable();
    }
    return false;
}

bool IOReaderPrivate::isSequential() {
    if(m_IODevice) {
        return m_IODevice->isSequential();
    }
    return false;
}

qint64 IOReaderPrivate::read(char *buffer) {
    qint64 read = 0;
    auto prev = m_IODevice->pos();
    if(!m_IODevice->seek(n_FilePointerPos)) {
        m_IODevice->seek(prev);
        return -1;
    }
    read = m_IODevice->read(buffer, n_BlockSize);
    n_FilePointerPos = m_IODevice->pos();
    m_IODevice->seek(prev);
    return read;
}

qint64 IOReaderPrivate::seek(qint64 offset, int whence) {
    switch(whence) {
    case SEEK_SET:
        n_FilePointerPos = offset;
        break;
    case SEEK_CUR:
        n_FilePointerPos += offset;
        break;
    case SEEK_END:
        n_FilePointerPos = m_IODevice->size() + offset;
        break;
    default:
        break;
    }

    /*
     * We have to get the actual seeked position from the IODevice
     * then set it to its previous position. */
    auto prev = m_IODevice->pos();
    if(!m_IODevice->seek(n_FilePointerPos)) {
        m_IODevice->seek(prev);
        return -1;
    }
    n_FilePointerPos = m_IODevice->pos();
    m_IODevice->seek(prev);
    return n_FilePointerPos;

}
