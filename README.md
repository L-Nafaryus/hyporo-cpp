# hpr

## Installation from sources

### Prerequisites

- CMake version 3.16 (or newer)

### Configure 

``` 
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -B cmake-build-debug -S .
```

### Build

``` 
cmake --build cmake-build-debug --target hpr
```

### Install

``` 
cmake --install cmake-build-debug --component devel
```

The following install components are supported:
- `runtime` - runtime package (core shared libraries and `.dll` files on Windows* OS).
- `devel` - development package (header files, CMake integration files, library symbolic links, and `.lib` files on Windows* OS).

### Pack

``` 
cd cmake-build-debug[.gitignore](.gitignore)
cpack
```