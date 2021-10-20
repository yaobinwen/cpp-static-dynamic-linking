#!/bin/sh

# Exit immediately on error; echo the commands.
set -ex

# Load the build functions.
. ./build-libs.sh

# Build the static library for `simplecalc`.
build_static_lib "simplecalc"

# Build the static library for `simplestr`.
build_static_lib "simplestr"

# Create the executable via `-static`.
g++ main.cpp -static -L. -lsimplecalc -lsimplestr -o main-g++-static.exe

# Run the executable.
./main-g++-static.exe

# Create the executable by specifying the static libraries directly.
g++ main.cpp libsimplecalc.a libsimplestr.a -o main-g++-static.exe

# Run the executable.
./main-g++-static.exe
