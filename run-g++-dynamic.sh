#!/bin/sh

# Exit immediately on error; echo the commands.
set -ex

# Load the build functions.
. ./build-libs.sh

# Build the dynamic library for `simplecalc`.
build_dynamic_lib "simplecalc"

# Build the dynamic library for `simplestr`.
build_dynamic_lib "simplestr"

# Create the executable.
# The libraries are linked dynamically by default so we don't really have to
# specify any additional options.
g++ main.cpp -L. -lsimplecalc -lsimplestr -Wall -o main-g++-dynamic.exe

# Run the executable by providing the proper `LD_LIBRARY_PATH`:
LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH" ./main-g++-dynamic.exe

# Remove the `.so` files so we cannot run the executable successfully because
# it requires the existence of the shared libraries.
rm -f libsimplecalc.so libsimplestr.so

# Run the executable again but expect the error "cannot open shared object
# file: No such file or directory".
LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH" ./main-g++-dynamic.exe
