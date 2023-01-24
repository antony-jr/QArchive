#include <QIODevice>
#include <QString>
#include <qarchiveioreader_p.hpp>
#include <qarchiveutils_p.hpp>

extern "C" {
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
}

// Support older libarchive versions where la_ssize_t is not defined yet
#if ARCHIVE_VERSION_NUMBER <= 3001002
#define la_ssize_t __LA_SSIZE_T
#endif

/*
 * This function destructs struct archive which is set
 * in the read mode via Qt Smart Pointer.
*/
void ArchiveReadDestructor(struct archive *ar) {
    if(ar) {
        archive_read_close(ar);
        archive_read_free(ar);
    }
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
}

void ArchiveEntryDestructor(struct archive_entry *e) {
    if(e) {
        archive_entry_free(e);
    }
}

/*
 * Custom libarchive callbacks to handle QIODevice as the archive
 * input. */

// Private data structure which holds a QIODevice along with its buffer size
// to read at a time.
// This structure will be used as the client data for the archive callbacks.
using ClientData_t = std::pair<char*, QArchive::IOReaderPrivate*>;

namespace {
// This callback will be called on archive open.
// This callback is simply used to avoid segmentation fault when
// the programmer mistakenly gives a QIODevice that has not opened.
int archive_open_cb(struct archive*, void* data)
{
    auto p = static_cast<ClientData_t*>(data);
    if(!p) {
        // We surely need the reader handle to continue
        // any further.
        return ARCHIVE_FATAL;
    }
    if (!p->second->isOpen() || !p->second->isReadable() || !(p->first) || p->second->isSequential()) {
        return ARCHIVE_FATAL;
    }
    return ARCHIVE_OK;
}

int archive_close_cb(struct archive*, void* data)
{
    // Should not do anything to archive pointer since
    // its a private object inside some other class.
    // It will be managed automatically later.
    auto p = static_cast<ClientData_t*>(data);
    if (p->first) { // free any data that has been allocated.
        free(p->first);
    }
    delete (p->second); //  Delete IOReaderPrivate.
    free(p); // free the client data allocated on creation.
    return ARCHIVE_OK;
}

// This read callback is called whenever libarchive needs
// more data to crunch , this is very important since we have
// to read the data from QIODevice.
la_ssize_t archive_read_cb(struct archive*, void* data, const void** buffer)
{
    auto p = static_cast<ClientData_t*>(data);
    *buffer = p->first;
    return p->second->read(p->first);
}

// This is the most important callback function required for libarchive to work with
// QIODevice.
// This callback seeks the QIODevice with respect to whence which is the same as
// given in fseek and lseek.
//
// Without this function you cannot extract 7zip archives.
int64_t archive_seek_cb(struct archive*, void* data, int64_t request, int whence)
{
    auto p = static_cast<ClientData_t*>(data);
    return static_cast<int64_t>(p->second->seek(request, whence));
}
/* ---- */

/*
 * Custom libarchive callbacks to handle QIODevice as the archive
 * output. */

int archive_w_open_cb(struct archive*, void* data)
{
    auto p = static_cast<QIODevice*>(data);
    if(!p) {
        return ARCHIVE_FATAL;
    }
    if(!p->isOpen() && !p->open(QIODevice::WriteOnly)) {
        return ARCHIVE_FATAL;
    }
    return ARCHIVE_OK;
}

int archive_w_close_cb(struct archive*, void* data)
{
    auto p = static_cast<QIODevice*>(data);
    if(!p) {
        return ARCHIVE_FATAL;
    }
    p->close();
    return ARCHIVE_OK;
}

la_ssize_t archive_write_cb(struct archive*, void* data, const void* buffer, size_t length)
{
    auto p = static_cast<QIODevice*>(data);
    return p->write(static_cast<const char*>(buffer), length);
}
} // namespace

// This is a custom functions which sets up the callbacks and other
// stuff for a libarchive struct.
int archiveReadOpenQIODevice(struct archive* archive, int blocksize, QIODevice* device)
{
    // This client data will be freed on close ,
    // we don't need to worry about this.
    auto p = static_cast<ClientData_t*>(calloc(1, sizeof(ClientData_t)));
    p->second = new QArchive::IOReaderPrivate;
    p->second->setIODevice(device);
    p->second->setBlockSize(blocksize);
    p->first = static_cast<char*>(calloc(1, (blocksize < 1024) ? sizeof(*(p->first)) * 10204 : sizeof(*(p->first)) * blocksize));

    archive_read_set_open_callback(archive, archive_open_cb);
    archive_read_set_read_callback(archive, archive_read_cb);
    archive_read_set_seek_callback(archive, archive_seek_cb);
    archive_read_set_close_callback(archive, archive_close_cb);
    archive_read_set_callback_data(archive, p);
    return archive_read_open1(archive);
}

// This is a custom functions which sets up the callbacks and other
// stuff for a libarchive struct to use QIODevice to write.
int archiveWriteOpenQIODevice(struct archive *archive, QIODevice *device) {
#if ARCHIVE_VERSION_NUMBER < 3005000
    return archive_write_open(archive, device, archive_w_open_cb,
                              archive_write_cb, archive_w_close_cb);
#else
/// Should not delete the client data because it's our QIODevice
/// buffer.
    return archive_write_open2(
        archive, device, archive_w_open_cb, archive_write_cb,
        archive_w_close_cb,
        [](struct archive *, void *) { return ARCHIVE_OK; });
#endif
}
/* ---- */

/*
 * This function returns an allocated c string which is the combination
 * of the given c strings.
 * Automatically allocates space for the new c string but does not
 * free it automatically.
*/
char *concat(const char *dest, const char *src) {
    auto ret = static_cast<char*>(calloc(sizeof(char), strlen(dest) + strlen(src) + 1));
    strcpy(ret, dest);
    strcat(ret, src);
    return ret;
}

/*
 * This function converts a string from "/home/antonyjr/"
 * to "/home/antonyjr" , i.e Removes the trailing '/' if found.
*/
QString getDirectoryFileName(const QString &dir) {
   if(dir.isEmpty()) {
      return dir;
   }

   if (dir[dir.length() - 1] == '/') {
        return dir.mid(0, dir.length() - 1);
    }
    return dir;
}
