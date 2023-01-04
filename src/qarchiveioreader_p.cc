#include <qarchiveioreader_p.hpp>

using namespace QArchive;

IOReaderPrivate::~IOReaderPrivate() {
    if(isOpen() &&
            isReadable() &&
            !isSequential()) {
        // IMPORTANT: We need to make sure the IO Device
        // is at default when we close it finally.
        // Otherwise, this will cause silent errors when
        // we attempt use the same QIODevice.
        m_IODevice->seek(0);
    }
}

void IOReaderPrivate::setBlockSize(int blockSize) {
    n_BlockSize = (blockSize < 1024) ?
                  10204 : blockSize;
}

void IOReaderPrivate::setIODevice(QIODevice *io) {
    m_IODevice = io;
}

bool IOReaderPrivate::isOpen() const {
    if(m_IODevice) {
        return m_IODevice->isOpen();
    }
    return false;
}

bool IOReaderPrivate::isReadable() const {
    if(m_IODevice) {
        return m_IODevice->isReadable();
    }
    return false;
}

bool IOReaderPrivate::isSequential() const {
    if(m_IODevice) {
        return m_IODevice->isSequential();
    }
    return false;
}

qint64 IOReaderPrivate::read(char *buffer) {
    return m_IODevice ? m_IODevice->read(buffer, n_BlockSize) : -1;
}

qint64 IOReaderPrivate::seek(qint64 offset, int whence) {
    if (!m_IODevice) {
        return -1;
    }

    auto value = offset;

    // Whence can be
    // SEEK_SET - Simply sets the seek
    // SEEK_CUR - Seeks past n from the current pointed location
    // SEEK_END - Seeks past n from the end of the file.
    //
    // No need to care about SEEK_SET, it's set by default,
    // look out for two other options.

    switch(whence) {
    case SEEK_CUR:
        value += m_IODevice->pos();
        break;
    case SEEK_END:
        value += m_IODevice->size();
        break;
    default:
        break;
    }

    return m_IODevice->seek(value) ? value : -1;
}
