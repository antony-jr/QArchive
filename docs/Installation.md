---
id: Installation
title: Installing QArchive.
sidebar_label: Installation
---

Before installing QArchive , make sure you have the below mentioned dependencies installed ,particularly in Windows,
libarchive is very hard to setup. Make sure you install the latest release of libarchive to utilize all features of
QArchive(needs atleast libarchive v3.3.3 to use passwords).

## Dependencies

* [libarchive](https://github.com/libarchive/libarchive)   
* [Qt5 Framework](https://qt.io)


## Conan

*Thanks to [Eric Riff](https://github.com/ericriff)*.

Starting from version 2.0.1 `QArchive` is available on the [Conan C/C++ package manager](https://conan.io/), which means you can download prebuilt packages for all major platforms (Windows, Linux, macOS, etc).


## Getting from source

The recommended way to install QArchive is to clone the github repo into your project directory , if your
project also uses git then add it as a submodule.

```
 $ git clone https://github.com/antony-jr/QArchive
```

or as a git submodule ,

```
 $ cd YourProject
 $ git submodule add https://github.com/antony-jr/QArchive
```

## Installing it to System

This way you can include QArchive in your project with CMake's **find_package** (```find_package(QArchive)```), without having to add QArchive to your git repo. This way your project CI/CD will only have to install QArchive to the system.


```
 $ git clone https://github.com/antony-jr/QArchive
 $ cd QArchive
 $ cmake .
 $ make -j$(nproc)
 $ sudo make install 
```
