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
        "mkdir"    : "QArchive",
        "install"  : {
            "QArchive.hpp" : "QArchive/QArchive.hpp",
            "LICENSE"      : "QArchive/LICENSE"
        }
}

def installPackage(config):
    print("Installing " + config["repo"])
    print("Creating Directory " + config["mkdir"])
    if os.path.exists(config["mkdir"]):
        rmtree(config["mkdir"])
    os.mkdir(config["mkdir"]) # Make the directory!
    print("Downloading the latest release from github... ")

    # Write files from the repo
    for i in config["install"]:
        resp = requests.get("https://raw.githubusercontent.com/"+config["username"]+"/"+config["repo"]+"/master/" + i)
        fp = open(config["install"][i] , "wb")
        for it in resp:
            fp.write(it)
        fp.close()

    print("Thank you for choosing "+config["repo"])
    print("Successfully Installed "+config["repo"] + "!")
    return True

if __name__ == "__main__":
    installPackage(QArchive)
    sys.exit(0)
