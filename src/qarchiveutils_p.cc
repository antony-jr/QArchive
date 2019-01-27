#include <qarchiveutils_p.hpp>
#include <QString>

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
void ArchiveReadDestructor(struct archive *ar)
{
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
void ArchiveWriteDestructor(struct archive *aw)
{
    if(aw) {
        archive_write_close(aw);
        archive_write_free(aw);
    }
    return;
}

void ArchiveEntryDestructor(struct archive_entry *e)
{
    if(e) {
        archive_entry_free(e);
    }
    return;
}

/*
 * This function returns an allocated c string which is the combination
 * of the given c strings.
 * Automatically allocates space for the new c string but does not
 * free it automatically.
*/
char *concat(const char *dest, const char *src)
{
    char *ret = (char*) calloc(sizeof(char), strlen(dest) + strlen(src) + 1);
    strcpy(ret, dest);
    strcat(ret, src);
    return ret;
}

/*
 * This function converts a string from "/home/antonyjr/"
 * to "/home/antonyjr" , i.e Removes the trailing '/' if found.
*/
QString getDirectoryFileName(const QString &dir)
{
    if(dir[dir.count() - 1] == QStringLiteral("/")) {
        return dir.mid(0, dir.count() - 1);
    }
    return dir;
}
