# 
# BSD-3 Clause.
# Copyright (C) 2017 Antony Jr.
# 
# A Simple Installer Script for Linux Users.

isCurl=$(command -v curl);
packageName="QArchive"
license="LICENSE"
repoRawUrl="https://github.com/antony-jr/QArchive/blob/master/"

if [ $isCurl ]
	then
		echo "Installing " $packageName
		echo "Creating directory:: " $packageName
		mkdir $packageName
		echo "Downloading LICENSE and the latest files... "
		curl -L $repoRawUrl$packageName".hpp" --output $packageName"/"$packageName".hpp"
		curl -L $repoRawUrl$license --output $packageName"/"$license
		echo "Installation complete!"
		echo "Thank you for choosing "$packageName
		exit
		
	else
		echo "Please install curl to continue the installation."
		echo "Aborting... "
		exit
fi
