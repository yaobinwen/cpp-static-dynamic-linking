#!/bin/sh

set -ex

test -d build || mkdir build

cd build

cmake ..

cmake --build . --verbose
