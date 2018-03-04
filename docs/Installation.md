---
id: Installation
title: Installing QArchive to your Project.
sidebar_label: Installation
---

**QArchive** is a small header writen in C++ using Qt5 Framework and **libarchive**. This   
little header helps you to **extract , compress and read** archives supported by **bsdtar** or   
**libarchive**

**QArchive** also supports the **Qt** event loop and does not block your main thread and thus   
this little header is a perfect fit for your Qt Projects.

**Before jumping the gun , make sure you have the [dependencies](#dependencies)**

## Dependencies

* [libarchive](https://github.com/libarchive/libarchive)   

	Before using **QArchive** make sure you have installed **libarchive** , its   
	kinda of a famous **library** so most of the linux distros have it listed   
	in their **repository**.

* [Qt5 Framework](https://qt.io)
	
	Qt is an amazing **GUI Framework** which is made for the future , it got   
	**stability , performance and the looks!**. I really recommend you all to   
	use Qt for your **GUI** Projects. And the best part is , its **Open Source**.   
	Make sure to install it too.


## Installing the latest release from github

Follow the instructions specific for your platform.

### Linux

**Just execute this command on your project folder and everything will be done for you!**   
This requires **requests** to be installed , so make sure you have installed **requests** for python.

```
 $ python -c "from requests import get;exec(get('https://git.io/vbbC1').content)"
```

**Thats it!**

### Windows

You need to have **python** with **requests** installed on your pc to do this but you can also clone   
the entire repo.

```
 > python -c "from requests import get;exec(get('https://git.io/vbbC1').content)"
```

### MacOSX

You need to have **python** with **requests** installed on your pc to do this but you can also clone  
the entire repo.

```
 $ python -c "from requests import get;exec(get('https://git.io/vbbC1').content)"
```
