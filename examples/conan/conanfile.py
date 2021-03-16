from conans import ConanFile, CMake

class Conan(ConanFile):
   name = "qarchiveConanExample"
   version = "1.0.0"
   settings = "arch"
   # By setting this this generator we instruct conan to generate the findPKG.cmake files.
   generators = "cmake_find_package"

   def configure(self):
      self.requires("qarchive/2.0.1")
