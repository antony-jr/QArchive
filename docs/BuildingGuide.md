---
id: BuildingGuide
title: Building Examples
sidebar_label: Building Examples
---

All examples are present at **examples** directory in the **repo** root. You can use CMake or QMake to compile all examples to try out QArchive and learn it through examples.

### CMake

```
 $ cd QArchive
 $ mkdir build
 $ cd build
 $ cmake -DBUILD_EXAMPLES=ON ..
 $ make -j$(nproc)
 $ # Should be compiled in examples/
```

### QMake

```
 $ cd QArchive/examples
 $ mkdir build
 $ cd build
 $ qmake ..
 $ make -j$(nproc)
```

