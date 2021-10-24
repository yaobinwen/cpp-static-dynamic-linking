#!/bin/sh

set -ex

g++ -o linker-demo.exe \
  ./linker/exceptions.cpp \
  ./linker/linker.cpp \
  ./linker/object_file.cpp \
  ./linker/processing.cpp \
  ./linker/static_lib.cpp \
  ./linker/symbol.cpp \
  linker-demo.cpp \
  -lboost_filesystem \
  -lboost_system \
  -lgtest \
  -lgtest_main \
  -lpthread

./linker-demo.exe
