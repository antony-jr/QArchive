TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = disk_extractor \
          disk_compressor \
	  disk_extractor_with_QIODevice \
	  memory_extractor \
	  memory_compressor
