---
id: AddingQArchive
title: Add QArchive to Your Qt Project
sidebar_label: Adding QArchive to Your Qt Project
---

|	    |				       |		
|-----------|----------------------------------|
|  Header:  | #include "QArchive/QArchive.hpp" |
|   qmake:  | QT += core		       |
|   	    | LIBS += -larchive		       |
|	    | HEADERS += QArchive/QArchive.hpp |

**QArchive** is just a header and all you have to do after installation is to add   
it in your **.pro** file ( **Qt Project file** ).

### Qt Project file (**.pro**)

Append these **lines** to your **Qt Project file**.

```
QT += core
LIBS += -larchive
HEADERS += QArchive/QArchive.hpp
```

### Including QArchive in your Source

Whenever you want to use **QArchive** , you just need to include it!

```
#include "QArchive/QArchive.hpp"
```

