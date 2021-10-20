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

# Create the executable by mixing static linking and dynamic linking.
g++ main.cpp -Wall -Wl,-Bstatic -lsimplecalc -Wl,-Bdynamic -L. -lsimplestr \
  -o main-g++-mix.exe

# Removing `libsimplecalc.so` should be OK because it's statically linked
rm -f libsimplecalc.so

# Run the executable.
LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH" ./main-g++-mix.exe

# Removing `libsimplestr.so` will cause run-time error because it's dynamically
# linked, but that is what we want to test.
rm -f libsimplestr.so

# Run the executable to confirm that we will get "error while loading shared
# libraries".
LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH" ./main-g++-mix.exe
