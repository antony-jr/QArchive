#!/usr/bin/env python3
import os

print("Write Conan, A Simple Script to Write Conanfile.py")
print("Copyright (C) 2023-Present, Antony J.R\n")

contents = '''#!/usr/bin/env python3
from conans import ConanFile, CMake

class Conan(ConanFile):
   name = "QArchive"
   version = "{ver}"
   settings = "os", "compiler", "build_type", "arch"
   generators = ["cmake_find_package", "cmake", "json"]
   
   options = {{"fPIC": [True, False]}}
   default_options = {{"fPIC": True}}

   def configure(self):
      self.requires("libarchive/3.6.2")
      self.requires("qt/{qt_ver}")
      self.requires("zlib/1.2.12")
      self.requires("zstd/1.5.5")

      self.options["qt"].shared = {build_shared}
      self.options["libarchive"].with_lz4 = True
      self.options["libarchive"].with_lzo = True
      self.options["libarchive"].with_lzma = True
      self.options["libarchive"].with_zstd = True
      self.options["libarchive"].with_bzip2 = True
'''

build_type = "Release"
build_shared = "True"
qarchive_version = "2.2.7"
qt_version = "5.15.2"

if 'BUILD_TYPE' in os.environ:
    build_type = os.environ['BUILD_TYPE']
if 'STATIC_BUILD' in os.environ:
    build_shared = "False"
if 'QARCHIVE_VERSION' in os.environ:
    qarchive_version = os.environ['QARCHIVE_VERSION']
if 'QT_VERSION' in os.environ:
    qt_version = os.environ['QT_VERSION']

print("BUILD_TYPE: {}".format(build_type))
print("SHARED BUILD: {}".format(build_shared))
print("QT_VERSION: {}".format(qt_version))
print("QARCHIVE_VERSION: {}".format(qarchive_version))

fp = open('conanfile.py', 'wt')
fp.write(contents.format(ver=qarchive_version, build_type=build_type, qt_ver=qt_version, build_shared=build_shared))
fp.close()

print("\nFile Written.")
