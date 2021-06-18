#include <qarchiveutils_p.hpp>
#include <qarchiveioreader_p.hpp>
#include <QString>
#include <QIODevice>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
}

/*
 * This function destructs struct archive which is set
 * in the read mode via Qt Smart Pointer.
*/
void ArchiveReadDestructor(struct archive *ar) {
    if(ar) {
        archive_read_close(ar);
        archive_read_free(ar);
    }
    return;
}

/*
 * This function destructs struct archive which is set in the
 * write mode via Qt Smart Pointer.
*/
void ArchiveWriteDestructor(struct archive *aw) {
    if(aw) {
        archive_write_close(aw);
        archive_write_free(aw);
    }
    return;
}

void ArchiveEntryDestructor(struct archive_entry *e) {
    if(e) {
        archive_entry_free(e);
    }
    return;
}

/*
 * Custom libarchive callbacks to handle QIODevice as the archive
 * input. */

// Private data structure which holds a QIODevice along with its buffer size
// to read at a time.
// This structure will be used as the client data for the archive callbacks.
struct ClientData_t {
    char *storage = nullptr;
    QArchive::IOReaderPrivate *io = nullptr;
};


// This callback will be called on archive open.
// This callback is simply used to avoid segmentation fault when
// the programmer mistakenly gives a QIODevice that has not opened.
static int archive_open_cb(struct archive *archive, void *data) {
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    if(!p) {
        // We surely need the reader handle to continue
        // any further.
        return ARCHIVE_FATAL;
    }
    if(!p->io->isOpen() ||
            !p->io->isReadable() ||
            !(p->storage) ||
            p->io->isSequential()) {
        return ARCHIVE_FATAL;
    }
    return ARCHIVE_OK;
}

static int archive_close_cb(struct archive *archive, void *data) {
    // Should not do anything to archive pointer since
    // its a private object inside some other class.
    // It will be managed automatically later.
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    if(p->storage) { // free any data that has been allocated.
        free(p->storage);
    }
    delete (p->io); //  Delete IOReaderPrivate.
    free(p); // free the client data allocated on creation.
    return ARCHIVE_OK;
}

// This read callback is called whenever libarchive needs
// more data to crunch , this is very important since we have
// to read the data from QIODevice.
static la_ssize_t archive_read_cb(struct archive *archive, void *data, const void **buffer) {
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    *buffer = (void*)p->storage;
    return p->io->read(p->storage);
}

// This is the most important callback function required for libarchive to work with
// QIODevice.
// This callback seeks the QIODevice with respect to whence which is the same as
// given in fseek and lseek.
//
// Without this function you cannot extract 7zip archives.
static int64_t archive_seek_cb(struct archive *archive, void *data, int64_t request, int whence) {
    Q_UNUSED(archive);
    ClientData_t *p = (ClientData_t*)data;
    return static_cast<int64_t>(p->io->seek(request, whence));
}

// This is a custom functions which sets up the callbacks and other
// stuff for a libarchive struct.
int archiveReadOpenQIODevice(struct archive *archive, int blocksize, QIODevice *device) {
    // This client data will be freed on close ,
    // we don't need to worry about this.
    ClientData_t *p = (ClientData_t*)calloc(1, sizeof *p);
    p->io = new QArchive::IOReaderPrivate;
    p->io->setIODevice(device);
    p->io->setBlockSize(blocksize);
    p->storage = (char*)calloc(1, (blocksize < 1024) ?
                               sizeof(*(p->storage)) * 10204 :
                               sizeof(*(p->storage)) * blocksize);

    archive_read_set_open_callback(archive, archive_open_cb);
    archive_read_set_read_callback(archive, archive_read_cb);
    archive_read_set_seek_callback(archive, archive_seek_cb);
    archive_read_set_close_callback(archive, archive_close_cb);
    archive_read_set_callback_data(archive, (void*)p);
    return archive_read_open1(archive);
}
/* ---- */

/*
 * Custom libarchive callbacks to handle QIODevice as the archive
 * output. */

static int archive_w_open_cb(struct archive *archive, void *data) {
    Q_UNUSED(archive);
    QIODevice *p = (QIODevice*)data;
    if(!p) {
        return ARCHIVE_FATAL;
    }
    if(!p->isOpen()) {
        if(!p->open(QIODevice::WriteOnly)) {
            return ARCHIVE_FATAL;
        }
    }
    return ARCHIVE_OK;
}

static int archive_w_close_cb(struct archive *archive, void *data) {
    Q_UNUSED(archive);
    QIODevice *p = (QIODevice*)data;
    if(!p) {
        return ARCHIVE_FATAL;
    }
    p->close();
    return ARCHIVE_OK;
}

static la_ssize_t archive_write_cb(struct archive *archive, void *data, const void *buffer, size_t length) {
    Q_UNUSED(archive);
    QIODevice *p = (QIODevice*)data;
    return p->write((const char*)buffer, length);
}

/// Should not delete the client data because it's our QIODevice
/// buffer.
int archive_w_free_cb(struct archive *archive, void *data) {
    Q_UNUSED(archive);
    Q_UNUSED(data);
    return ARCHIVE_OK;
}

// This is a custom functions which sets up the callbacks and other
// stuff for a libarchive struct to use QIODevice to write.
int archiveWriteOpenQIODevice(struct archive *archive, QIODevice *device) {
    return archive_write_open2(archive,
                               (void*)device,
                               archive_w_open_cb,
                               archive_write_cb,
                               archive_w_close_cb,
                               archive_w_free_cb);
}
/* ---- */

/*
 * This function returns an allocated c string which is the combination
 * of the given c strings.
 * Automatically allocates space for the new c string but does not
 * free it automatically.
*/
char *concat(const char *dest, const char *src) {
    char *ret = (char*) calloc(sizeof(char), strlen(dest) + strlen(src) + 1);
    strcpy(ret, dest);
    strcat(ret, src);
    return ret;
}

/*
 * This function converts a string from "/home/antonyjr/"
 * to "/home/antonyjr" , i.e Removes the trailing '/' if found.
*/
QString getDirectoryFileName(const QString &dir) {
    if(dir[dir.count() - 1] == QStringLiteral("/")) {
        return dir.mid(0, dir.count() - 1);
    }
    return dir;
}
