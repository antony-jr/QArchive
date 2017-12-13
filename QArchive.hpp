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
 *  @filename 	 	: QArchive.h
 *  @description 	: C++ Cross-Platform helper library that Modernizes libarchive
 *  			  using Qt5. Simply extracts 7z  , Tarballs  , RAR
 *  			  and other supported formats by libarchive.
 * -----------------------------------------------------------------------------
*/
#if !defined (QARCHIVE_HPP_INCLUDED)
#define QARCHIVE_HPP_INCLUDED

#include <QtCore>
#include <QString>
#include <QThread>
#include <QStringList>

/*
 * Getting the libarchive headers for the
 * runtime operating system
*/
extern "C" {
#include <sys/types.h>
#include <archive.h>
#include <archive_entry.h>
}

#define NO_ARCHIVE_ERROR 0
#define ARCHIVE_QUALITY_ERROR 1
#define ARCHIVE_READ_ERROR 2
#define ARCHIVE_UNCAUGHT_ERROR 3

namespace QArchive { // QArchive Namespace Start
	/*
	 * Class Extractor <- Inherits QObject
	 * ---------------
	 *
	 *  Takes care of extraction of archives with the help
	 *  of libarchive.
	 *
	 *  Constructors:
	 *  	Extractor(QString&) 	- Simply extract a single archive , any format
	 *  			      	  , Most likely will be used for 7zip.
	 *  	Extractor(QStringList&)	- Extract all archives from the QStringList in
	 *  				  order.
	 *  	Extractor(void)		- Do nothing just construct!
	 *  Methods:
	 *	void addArchive(QString&)     - Add a archive to the queue.
	 *	void addArchive(QStringList&) - Add a set of archives to the queue
	 *	void removeArchive(QString&)  - Removes a archive from the queue matching
	 *					the QString.
	 *  
	 *  Slots:
	 *	int run(void)		      - starts the extractor.
	 *	int stop(void)		      - stops the extractor.
	 *  
	 *  Signals:
	 *	void finished(bool , QString) - emitted when a extraction job is done or
	 *					 shows a error when the bool is false!
	 *					 on true , gives the archive name.
	 *
	*/
	class Extractor  : public QThread 
	{
		Q_OBJECT
		
	   public:
		Extractor(QObject *parent = NULL){ }

		Extractor(QString& filename)
		{
			addArchive(filename);
		}

		Extractor(QStringList& filenames)
		{
			addArchive(filenames);
		}

		void addArchive(QString& filename)
		{       
        		queue << filename;
        		queue.removeDuplicates();
		}

		void addArchive(QStringList& filenames)
		{
        
        	/*
         	 * No need to extract the same archive twice!
        	*/
        	queue << filenames;
        	queue.removeDuplicates();
		}

		void removeArchive(QString& filename)
		{       
        	queue.removeAll(filename);
		}

		void run() override {
        	for(auto i = 0; i < queue.size(); ++i)
        	{
                	if(extract(queue.at(i).toStdString().c_str())){
                        	queue.removeAll(queue.at(i));
                        	return;
                	}
			emit extracted(QString(queue.at(i)));
        	}
        	queue.clear();
        	emit finished();
		return;
		}

		~Extractor() { }
	   signals:
	   	void finished();
		void extracted(const QString);
		void error(short);
	   private:
		QStringList queue;

		struct archive *arch,
			       *ext;
		struct archive_entry *entry;
		int ret;

		int extract(const char* filename){
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

		int copy_data()
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

	};
	
	/*
         * Class Reader <- Inherits QObject
         * ------------
         *
         *  Reads archives using libarchive. Simply returns a QStringList
	 *  of files and folders , does not support showing subfolders.
	 *  Just to verify files maybe ?
	 *
         *  Constructors:
         *
         *  Methods:
         *
        
	class Reader	 : public QObject
	{
		Q_OBJECT
	};	
	
	 *
         * Class Compressor <- Inherits QObject
         * ----------------
         *
         *  Compresses files and folders using libarchive.
         *
         *  Constructors:
         *
         *  Methods:
         *
        
	class Compressor : public QObject
	{
		Q_OBJECT
	};
	*/

} // QArchive Namespace Ends.



#endif // QARCHIVE_HPP_INCLUDED
