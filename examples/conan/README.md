# QArchive & Conan example

## Intro
Starting version 2.0.1 `QArchive` is available on the [Conan C/C++ package manager](https://conan.io/), which means you can download prebuilt packages for all major platforms (Windows, Linux, macOS, etc).

## Build
The CMakeLists in this example will pull the `conan.cmake` module, which will automate all the Conan calls. That snippet can be replaced with manual Conan calls if desired.

```bash
pip install conan

mkdir build && cd build

cmake .. && cmake --build -jn .
./quarchiveProject
```
## Notice
1. The `-fPIC` compile option is needed to use Conan's qt5
2. Components are not supported on Conan's qt5 (yet), so the whole qt package is linked against.
3. `CMAKE_AUTOMOC` doesn't currently work with Conan's qt5. Use `qt5_wrap_cpp()` in te mean time.
