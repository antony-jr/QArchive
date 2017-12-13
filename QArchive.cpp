/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2017, Antony jr
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 * * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------------
 *  @filename 	 	: QArchive.cpp
 *  @description 	: C++ Cross-Platform helper library that Modernizes libarchive
 *  			  using Qt5. Simply extracts 7z  , Tarballs  , RAR
 *  			  and other supported formats by libarchive.
 * -----------------------------------------------------------------------------
*/

#include "QArchive.hpp"
#include <QDebug>

/*
 * Class Extractor Declaration.
*/


void QArchive::Extractor::addArchive(QString& filename)
{
	queue << filename;
	queue.removeDuplicates();
}

void QArchive::Extractor::addArchive(QStringList& filenames)
{

   	/*
         * No need to extract the same archive twice!
        */
        queue << filenames;
        queue.removeDuplicates();
}

void QArchive::Extractor::removeArchive(QString& filename)
{
	queue.removeAll(filename);
}

int QArchive::Extractor::extract(const char* filename){
	arch = archive_read_new();
        ext = archive_write_disk_new();
        archive_write_disk_set_options(ext, ARCHIVE_EXTRACT_TIME);
        archive_read_support_format_all(arch);

	if((ret = archive_read_open_filename(arch, filename, 10240))){
		emit error(ARCHIVE_READ_ERROR);
		return ret;
	}
	for (;;) {
                ret = archive_read_next_header(arch, &entry);
                if (ret == ARCHIVE_EOF){ break; }
                if (ret != ARCHIVE_OK){
			emit error(ARCHIVE_QUALITY_ERROR);
                        return 1;
		}
                ret = archive_write_header(ext, entry);
                if (ret == ARCHIVE_OK){
                          copy_data();
                          ret = archive_write_finish_entry(ext);
                          if (ret != ARCHIVE_OK){
                              emit error(ARCHIVE_UNCAUGHT_ERROR);
			      return -1;
			  }
                }

        }
	archive_read_close(arch);
        archive_read_free(arch);

        archive_write_close(ext);
        archive_write_free(ext);
	
	return 0;
}

int QArchive::Extractor::copy_data()
{
        const void *buff;
        size_t size;
#if ARCHIVE_VERSION_NUMBER >= 3000000
        int64_t offset;
#else
        off_t offset;
#endif

        for (;;) {
                ret = archive_read_data_block(arch, &buff, &size, &offset);
                if (ret == ARCHIVE_EOF)
                        return (ARCHIVE_OK);
                if (ret != ARCHIVE_OK)
                        return (ret);
                ret = archive_write_data_block(ext, buff, size, offset);
                if (ret != ARCHIVE_OK) {
                        return (ret);
                }
        }
}
