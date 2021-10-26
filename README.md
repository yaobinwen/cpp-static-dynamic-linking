# C++ Static and Dynamic Linking

## 1. Overview

This repository has code to demonstrate:
- How to link libraries statically and dynamically using `g++` or `CMake`.
- How to resolve the linking errors when linking `boost::log` statically.
- How the linker resolves the symbols when linking object files and **static** libraries.

## 2. Static & Dynamic Linking

The code is in the current directory.

The `run-g++` shell scripts show how to link libraries in different ways:

| Script | Description |
|-------:|:------------|
| run-g++-both-force-static.sh | How to force static linking when both static and dynamic libraries are present. |
| run-g++-dynamic.sh | How to link dynamic libraries only. |
| run-g++-mix.sh | How to mix the static and dynamic linking. |
| run-g++-static.sh | How to link static libraries only. |

The `CMakeLists.txt` has code that does the same using `CMake`.

## 3. Static Linking Errors in `boost::log`

This sub-directory addresses the problem that building programs with `boost::log` in `CMake` with static linking may result in link errors.

The code is in the sub-directory `boost_log`. See the `README.md` for more details.

## 4. Library Order in Static Linking

This sub-directory is an additional resource of understanding the article [Library order in static linking](https://eli.thegreenplace.net/2013/07/09/library-order-in-static-linking) which helps you to understand why link errors happens on static libraries and how to debug them.

The code is in the sub-directory `library-order-in-static-linking`. See the `README.md` for more details.
