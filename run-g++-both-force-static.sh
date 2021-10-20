#!/bin/sh

# Exit immediately on error; echo the commands.
set -ex

# Load the build functions.
. ./build-libs.sh

# Build the static library for `simplecalc`.
build_static_lib "simplecalc"

# Build the static library for `simplestr`.
build_static_lib "simplestr"

# Build the dynamic library for `simplecalc`.
build_dynamic_lib "simplecalc"

# Build the dynamic library for `simplestr`.
build_dynamic_lib "simplestr"

# Failed attempts.
(
  set +e

  # Error messages:
  # - /usr/bin/ld: cannot find -lsimplecalc
  # - /usr/bin/ld: cannot find -lsimplestr
  g++ main.cpp -Wall \
    -static-libgcc -static-libstdc++ \
    -Wl,-Bstatic -lsimplecalc -lsimplestr \
    -o main-g++-both-force-static.exe

  # Error messages:
  # - /usr/bin/ld: cannot find -lsimplecalc
  # - /usr/bin/ld: cannot find -lsimplestr
  g++ main.cpp -Wall \
    -static-libgcc -static-libstdc++ \
    -Wl,-Bstatic -lsimplecalc \
    -Wl,-Bstatic -lsimplestr \
    -o main-g++-both-force-static.exe
) || true # OR with `true` so the failed attempts don't fail the entire script.

# Create the executable by forcing static linking even if the shared libraries
# are also available.
g++ main.cpp -Wall \
    -static-libgcc -static-libstdc++ \
    -Wl,-Bstatic -L. -lsimplecalc -lsimplestr \
    -o main-g++-both-force-static.exe

# Create the executable by forcing static linking even if the shared libraries
# are also available.
g++ main.cpp -Wall -static -Wl,-L. -lsimplecalc -lsimplestr \
  -o main-g++-both-force-static.exe

# Remove the shared library to test if the executable was linked against the
# static library.
rm -f libsimplecalc.so libsimplestr.so

# Run the executable.
LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH" ./main-g++-both-force-static.exe
