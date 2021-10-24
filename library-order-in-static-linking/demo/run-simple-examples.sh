#!/bin/sh

set -ex

g++ -o simple-examples.exe \
  ./linker/exceptions.cpp \
  ./linker/linker.cpp \
  ./linker/object_file.cpp \
  ./linker/processing.cpp \
  ./linker/static_lib.cpp \
  ./linker/symbol.cpp \
  simple-examples.cpp \
  -lboost_filesystem \
  -lboost_system \
  -lgtest \
  -lgtest_main \
  -lpthread

./simple-examples.exe
