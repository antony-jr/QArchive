#!/usr/bin/python
#
# BSD-3 Clause.
# Copyright (C) 2017 Antony Jr.
#
# Simple Cross Platform Installer Script
import sys
import os
import requests
from shutil import rmtree

# Packages to install
QArchive = {
        "username" : "antony-jr",
        "repo"     : "QArchive",
        "mkdir"    : {
            "QArchive/src",
            "QArchive/include"
        },
        "install"  : {
            "CMakeLists.txt"       : "QArchive/CMakeLists.txt",
            "QArchive.pro"         : "QArchive/QArchive.pro",
            "QArchive"             : "QArchive/QArchive",
            "include/QArchive.hpp" : "QArchive/include/QArchive.hpp",
            "src/QArchive.cc"      : "QArchive/src/QArchive.cc",
            "LICENSE"      : "QArchive/LICENSE"
        }
}

def installPackage(config):
    print("Installing " + config["repo"])
    # Make parent directory first.
    if os.path.isfile(config["repo"]):
        print("Deleting duplicate file(s)... ")
        os.remove(config["repo"])
        os.mkdir(config["repo"])
    else:
     if os.path.exists("QArchive"):
         rmtree("QArchive")
     else:
         os.mkdir("QArchive")

    for i in config["mkdir"]:
     print("Creating Directory " + i)
     if os.path.exists(i):
         rmtree(i)
     os.mkdir(i) # Make the directory!
    print("Downloading the latest release from github... ")

    # Write files from the repo
    for i in config["install"]:
        resp = requests.get("https://raw.githubusercontent.com/"+config["username"]+"/"+config["repo"]+"/master/" + i)
        fp = open(config["install"][i] , "wb")
        for it in resp:
            fp.write(it)
        fp.close()

    print("Installed "+config["repo"]+".")
    return True

if __name__ == "__main__":
    installPackage(QArchive)
    sys.exit(0)
